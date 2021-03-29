#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <MsTimer2.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "wifi.h"
#define wifi_led A0


SoftwareSerial mySerial(11, 10); // RX, TX
Adafruit_SHT31 sht31 = Adafruit_SHT31();

int time_cnt = 0, cnt = 0, init_flag = 0;
int turns;
int bright_number;
int fan_number;
int TEM = 0, HUM = 0;
int tem_up_a;
int water_press_a;

void setup() {
  pinMode(2, INPUT_PULLUP);//跑轮测速
  pinMode(3, OUTPUT);//蜂鸣器
  pinMode(4, OUTPUT);//加热片
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);//LED_PWM
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);//风扇_PWM
  digitalWrite(6, LOW);
  pinMode(8, INPUT);//风扇开关
  pinMode(9, OUTPUT);//水泵_PWM
  digitalWrite(9, LOW);
  pinMode(12, INPUT);//LED开关
  pinMode(A0, OUTPUT);//网络指示灯
  pinMode(A1, OUTPUT);//加热LED
  digitalWrite(A1, HIGH);
  pinMode(A2, INPUT);//加热开关
  pinMode(A3, INPUT_PULLUP);//重置按钮
  sht31.begin(0x44);//SHT31初始化
  attachInterrupt(0, count, FALLING);//硬件中断
  MsTimer2::set(5000, mcu_get_green_time);//读取时间中断
  Serial.begin(9600);//和上板通信
  mySerial.begin(9600);//和涂鸦模块通信
  tem_up_a = EEPROM.read(10);//读取最高温度设定值
  if (tem_up_a == 255) tem_up_a = 30;
  water_press_a = EEPROM.read(20);//读取设定水泵速度1-10
  if (water_press_a == 255) water_press_a = 10;
  fan_number = EEPROM.read(30);//读取风扇速度1-10
  if (fan_number == 255) fan_number = 10;
  bright_number = EEPROM.read(40);//读取亮度1-10
  if (bright_number == 255) bright_number = 10;
  wifi_protocol_init();
}

void loop() {
  if (init_flag == 0) {
    time_cnt++;
    if (time_cnt % 6000 == 0) {
      time_cnt = 0;
      cnt ++;
    }
    wifi_stat_led(&cnt);   // Wi-Fi状态处理
  }
  wifi_uart_service();
  myserialEvent();      // 串口接收处理
  key_scan();           // 重置配网按键检测
  if (init_flag == 1) //联网后执行
  {
    control();
  }
}

int t = 0, tag = 0;
int TEM_old = 0, HUM_old = 0, turns_old = 0;
int state_1 = 1, state_2 = 1, state_3 = 1;
int Before_1 = 0, Before_2 = 0, Before_3 = 0;
int fan_1 = 0, auto_hot = 0;
unsigned char times[7]= {0};

void control() { //联网后执行循环
  if(tag == 0){
  MsTimer2::start(); //打开时间中断
  tag = 1;
  }
  t++;
  if (t == 500 || t == 1000) {//温度读取
    TEM = (int)(sht31.readTemperature());
    HUM = (int)sht31.readHumidity();
  }
  if (TEM != TEM_old) mcu_dp_value_update(DPID_TEMP_CURRENT, TEM); //VALUE型数据上报
  if (HUM != HUM_old) mcu_dp_value_update(DPID_HUMIDITY_CURRENT, HUM); //VALUE型数据上报
  TEM_old = TEM;
  HUM_old = HUM;
  //  ******************************************LED控制
  int Bstate_1 = digitalRead(12);
  if (Bstate_1 && Before_1 == 0) {
    digitalWrite(5, state_1);
    mcu_dp_bool_update(DPID_LIGHT, state_1); //BOOL型数据上报;
    state_1 = !state_1;
    Before_1 = !Before_1;
  } else if (Bstate_1 == 0 && Before_1 == 1) {
    Before_1 = !Before_1;
  }
  //  ******************************************风扇开关
  int Bstate_2 = digitalRead(8);
  if (Bstate_2 && Before_2 == 0) {
    if (fan_1 == 0) {
      digitalWrite(6, map(fan_number, 0, 10, 0, 254));
      mcu_dp_bool_update(DPID_LIGHT, 1); //BOOL型数据上报;
      fan_1 = !fan_1;
    } else {
      digitalWrite(6, 0);
      mcu_dp_bool_update(DPID_LIGHT, 0); //BOOL型数据上报;
      fan_1 = !fan_1;
    }
    state_2 = !state_2;
    Before_2 = !Before_2;
  } else if (Bstate_2 == 0 && Before_2 == 1) {
    Before_2 = !Before_2;
  }
  //  ******************************************加热控制
  int Bstate_3 = digitalRead(A2);
  if (Bstate_3 && Before_3 == 0) {
    digitalWrite(4, state_3);
    auto_hot = !auto_hot;
    mcu_dp_bool_update(DPID_SWITCH, state_3); //BOOL型数据上报;
    state_3 = !state_3;
    digitalWrite(A1, state_3); //指示灯
    Before_3 = !Before_3;
  } else if (Bstate_3 == 0 && Before_3 == 1) {
    Before_3 = !Before_3;
  }
  //  ******************************************防抖结束
  if(auto_hot == 1){
    //加热片热反馈处理
  }
  if(turns != turns_old){
  mcu_dp_value_update(DPID_CIRCLE, round(turns/10));
  turns_old = turns;
  }
  if(times[3] > 32) times[3] = 0;
   if (t == 1000) {
  Serial.print(TEM);
  Serial.print(",");
  Serial.print(HUM);
  Serial.print(",");
  Serial.print(times[3]);
  Serial.print(",");
  Serial.print(turns);
  t = 0;}
  
}

void count() //中断处理
{
  turns = turns + 1;
}

void myserialEvent() {
  if (mySerial.available()) {
    unsigned char ch = (unsigned char)mySerial.read();
    uart_receive_input(ch);
  }
}

void key_scan(void)//重置
{
  static char ap_ez_change = 0;
  unsigned char buttonState  = HIGH;
  buttonState = digitalRead(A3);
  if (buttonState == LOW) {
    delay(3000);
    buttonState = digitalRead(A3);
    if (buttonState == LOW) {
      init_flag = 0;
      switch (ap_ez_change) {
        case 0 :
          mcu_set_wifi_mode(SMART_CONFIG);
          break;
        case 1 :
          mcu_set_wifi_mode(AP_CONFIG);
          break;
        default:
          break;
      }
      ap_ez_change = !ap_ez_change;
    }
  }
}

void wifi_stat_led(int *cnt)// WiFi连接灯处理
{
  switch (mcu_get_wifi_work_state())
  {
    case SMART_CONFIG_STATE:  //0x00
      init_flag = 0;
      if (*cnt == 2) {
        *cnt = 0;
      }
      if (*cnt % 2 == 0)  //LED快闪
      {
        digitalWrite(wifi_led, LOW);
      }
      else
      {
        digitalWrite(wifi_led, HIGH);
      }
      break;
    case AP_STATE:  //0x01
      init_flag = 0;
      if (*cnt >= 30) {
        *cnt = 0;
      }
      if (*cnt  == 0)      // LED 慢闪
      {
        digitalWrite(wifi_led, LOW);
      }
      else if (*cnt == 15)
      {
        digitalWrite(wifi_led, HIGH);
      }
      break;

    case WIFI_NOT_CONNECTED:  // 0x02
      digitalWrite(wifi_led, LOW); // LED 熄灭
      break;
    case WIFI_CONNECTED:  // 0x03
      break;
    case WIFI_CONN_CLOUD:  // 0x04
      if ( 0 == init_flag )
      {
        digitalWrite(wifi_led, HIGH);// LED 常亮
        init_flag = 1;// Wi-Fi 连接上后该灯可控
        *cnt = 0;
      }
      break;

    default:
      digitalWrite(wifi_led, LOW);
      break;
  }
}
