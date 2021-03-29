



void SelectUnit(int ID)
{
  switch (ID)
  {
    case 1:
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 2:
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 3:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 4:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 5:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, LOW);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 6:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 7:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
    case 8:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
    default:
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;

  }
}

void ShowData(int UnitID, char * data)
{
  SelectUnit(UnitID);
  for (int i = 0; i < 8; i++)
  {
    if (data[i] == '0')
    {
      digitalWrite(i + 2, LOW);

    }
    else
    {
      digitalWrite(i + 2, HIGH);

    }

  }
}

void ShowNone()
{
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);

  for (int i = 2; i <= 9 ; i++)
  {
    digitalWrite(i, LOW);
  }
}


void Show(int UnitID, char a)
{
  ShowNone();
  switch (a)
  {
    case '0':
      ShowData(UnitID, "11111100");
      break;
    case '1':
      ShowData(UnitID, "01100000");
      break;
    case '2':
      ShowData(UnitID, "11011010");
      break;
    case '3':
      ShowData(UnitID, "11110010");
      break;
    case '4':
      ShowData(UnitID, "01100110");
      break;
    case '5':
      ShowData(UnitID, "10110110");
      break;
    case '6':
      ShowData(UnitID, "10111110");
      break;
    case '7':
      ShowData(UnitID, "11100000");
      break;
    case '8':
      ShowData(UnitID, "11111110");
      break;
    case '9':
      ShowData(UnitID, "11110110");
      break;
    case '.':
      ShowData(UnitID, "00000001");
      break;
    case '-':
      ShowData(UnitID, "00000010");
      break;
    default:
      ShowNone();
      break;


  }

  delay(2);
}

void ShowNum1(float val)
{
  char buf[9];
  for (int i = 0; i < 9; i++)
  {
    buf[i] = '\0';
  }
  dtostrf(val, 1, 3, buf);

  if (buf[1] == '.') //0.123
  {
    Show(1, buf[0]);
    Show(1, '.');
    Show(2, buf[2]);
    Show(3, buf[3]);
    Show(4, buf[4]);

  }

  if (buf[2] == '.') //11.12
  {
    Show(1, buf[0]);
    Show(2, buf[1]);
    Show(2, '.');
    Show(3, buf[3]);
    Show(4, buf[4]);

  }


  if (buf[3] == '.') //111.12
  {
    Show(1, buf[0]);
    Show(2, buf[1]);
    Show(3, buf[2]);
    Show(3, '.');
    Show(4, buf[4]);

  }

  if (buf[4] == '.') //1112.12
  {
    Show(1, buf[0]);
    Show(2, buf[1]);
    Show(3, buf[2]);
    Show(4, buf[3]);
    Show(4, '.');

  }
}

void ShowNum2(float val)
{
  char buf[9];
  for (int i = 0; i < 9; i++)
  {
    buf[i] = '\0';
  }
  dtostrf(val, 1, 3, buf);

  if (buf[1] == '.') //0.123
  {
    Show(5, buf[0]);
    Show(5, '.');
    Show(6, buf[2]);
    Show(7, buf[3]);
    Show(8, buf[4]);

  }

  if (buf[2] == '.') //11.12
  {
    Show(5, buf[0]);
    Show(6, buf[1]);
    Show(6, '.');
    Show(7, buf[3]);
    Show(8, buf[4]);

  }


  if (buf[3] == '.') //111.12
  {
    Show(5, buf[0]);
    Show(6, buf[1]);
    Show(7, buf[2]);
    Show(7, '.');
    Show(8, buf[4]);

  }

  if (buf[4] == '.') //1112.12
  {
    Show(5, buf[0]);
    Show(6, buf[1]);
    Show(7, buf[2]);
    Show(8, buf[3]);
    Show(8, '.');

  }
}

String comdata = "";
int numdata[4] = {0}, mark = 0;
int serial1 = 0;
int serial2 = 0;
int serial3 = 0;
int serial4 = 0;
void setup() {
  Serial.begin(9600);
  for (int i = 2; i <= 19; i++)
  {
    pinMode(i, OUTPUT);
  }
}
int times = 0;
//int days = 0;
int old_days = 0;
int marks = 0;
void loop() {

  int j = 0;
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }
  if (mark == 1)
  {
    for (int i = 0; i < comdata.length() ; i++)
    {
      if (comdata[i] == ',')
      {
        j++;
      }
      else
      {
        numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
      }
    }
    comdata = String("");
    mark = 0;
//    days++;
    serial1 = numdata[0];
    serial2 = numdata[1];
    serial3 = numdata[2];
    serial4 = numdata[3];
    if (serial3 != 0 && marks == 0) {
      old_days = serial3;
      marks = 1;
    }
    for (int i = 0; i < 4; i++)
    {
      numdata[i] = 0;
    }
  }
  times++;
  if (times < 300) {
    digitalWrite(18, HIGH);
    ShowNum1(serial1);
    ShowNum2(serial2);
  } else {
    digitalWrite(18, LOW);
    ShowNum1(serial3 - old_days);
    ShowNum2(serial4);
  }
  if (times > 600)times = 0;
  if (serial3 - old_days > 15) {
    digitalWrite(19, LOW);
  } else {
    digitalWrite(19, HIGH);
  }
}
