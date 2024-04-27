#include <LiquidCrystal_I2C.h>
#include<AD9833.h>

#define rootOfTen (sqrt(sqrt(10)))

LiquidCrystal_I2C lcd(0x27, 16, 2);
AD9833 gen(10);


bool startStatus = false;
int clk = 2;
int dt = 3;
int sw = 4;
int button1 = 5;
int button2 = 6;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
int wave = 0;
int menu = 0;
volatile double precisionScale = 1;

void sine()
{
  
  byte sine0[] = {0b00001, 0b00110, 0b01000, 0b10000, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte sine1[] = {0b11000, 0b00110, 0b00001, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte sine2[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b10000, 0b01000, 0b00110, 0b00001 };
  byte sine3[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00001, 0b00110, 0b11000 };
  byte sine4[] = {0b00001, 0b00110, 0b01000, 0b10000, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte sine5[] = {0b11000, 0b00110, 0b00001, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
  lcd.createChar(0, sine0);
  lcd.createChar(1, sine1);
  lcd.createChar(2, sine2);
  lcd.createChar(3, sine3);
  lcd.createChar(4, sine4);
  lcd.createChar(5, sine5);
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.write(byte(2));
  lcd.setCursor(13, 0);
  lcd.write(byte(3));
  lcd.setCursor(14, 0);
  lcd.write(byte(4));
  lcd.setCursor(15, 0);
  lcd.write(byte(5));

}

void square()
{
  
  byte square0[] = {0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000 };
  byte square1[] = {0b11000, 0b01000, 0b01000, 0b01000, 0b01000, 0b01000, 0b01000, 0b01111 };
  byte square2[] = {0b00111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b11100 };
  byte square3[] = {0b11110, 0b00010, 0b00010, 0b00010, 0b00010, 0b00010, 0b00010, 0b00011 };
  byte square4[] = {0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b11111 };
  byte square5[] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };
  lcd.createChar(0, square0);
  lcd.createChar(1, square1);
  lcd.createChar(2, square2);
  lcd.createChar(3, square3);
  lcd.createChar(4, square4);
  lcd.createChar(5, square5);
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.write(byte(2));
  lcd.setCursor(13, 0);
  lcd.write(byte(3));
  lcd.setCursor(14, 0);
  lcd.write(byte(4));
  lcd.setCursor(15, 0);
  lcd.write(byte(5));

}

void triangle()
{
  
  byte triangle0[] = {0b00000, 0b00000, 0b00000, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000 };
  byte triangle1[] = {0b01000, 0b10100, 0b00010, 0b00001, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte triangle2[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b10001, 0b01010, 0b00100 };
  byte triangle3[] = {0b00010, 0b00101, 0b01000, 0b10000, 0b00000, 0b00000, 0b00000, 0b00000 };
  byte triangle4[] = {0b00000, 0b00000, 0b00000, 0b10000, 0b01000, 0b00100, 0b00010, 0b00001 };
  byte triangle5[] = {0b00000, 0b00000, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b00000 };
  lcd.createChar(0, triangle0);
  lcd.createChar(1, triangle1);
  lcd.createChar(2, triangle2);
  lcd.createChar(3, triangle3);
  lcd.createChar(4, triangle4);
  lcd.createChar(5, triangle5);
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.write(byte(2));
  lcd.setCursor(13, 0);
  lcd.write(byte(3));
  lcd.setCursor(14, 0);
  lcd.write(byte(4));
  lcd.setCursor(15, 0);
  lcd.write(byte(5));

}

void setup() {
  lcd.init();
  lcd.backlight();
  startupScreen();
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  digitalWrite(clk, HIGH); 
  digitalWrite(dt, HIGH); 


  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
  gen.Begin();
}

void startupScreen() {
  lcd.setCursor(0, 0);
  lcd.print(" DDS   FUNCTION ");
  lcd.setCursor(0, 1);
  lcd.print("GENERATOR INIT..");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (digitalRead(button1) == LOW) 
   {
     changeEncoderMenu();
   }
  startFunc();

  if (menu == 0) 
  {
    lcd.setCursor(0, 1);
    lcd.print("Freq: ");
    lcd.print(encoderValue / 4);
    lcd.print("Hz");
    lcd.print("        ");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Scale: ");
    lcd.print(precisionScale, 0);
    lcd.print("Hz");
    lcd.print("       ");
  }

  if (digitalRead(sw) == LOW) 
  {
    wave ++;
    wave %= 3;
  }

  lcd.setCursor(4, 0);
  lcd.print("Wave: ");
  switch (wave) {
    case 0:
      {
        square();
        break;
      }
    case 1:
      {
        sine();
        break;
      }
    case 2:
      {
        triangle();
        break;
      }
    default:
      break;
  }

  if (startStatus == false) 
  {
    gen.EnableOutput(false);
    lcd.setCursor(0, 0);
    lcd.print("OFF");
  }
  else if (startStatus == true && encoderValue != 0)
  {
    gen.EnableOutput(true);
    lcd.setCursor(0, 0);
    lcd.print("ON ");

    switch (wave) {
      case 0:
        {
          gen.ApplySignal(SQUARE_WAVE, REG0, encoderValue / 4);
          break;
        }
      case 1:
        {
          gen.ApplySignal(SINE_WAVE, REG0, encoderValue / 4);
          break;
        }
      case 2:
        {
          gen.ApplySignal(TRIANGLE_WAVE, REG0, encoderValue / 4);
          break;
        }
      default:
        break;
    }

  }

  delay(200); 
}

void startFunc()
{
  if (digitalRead(button2) == LOW) 
  {
    startStatus = not startStatus;
  }
}

void changeEncoderMenu()
{
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  if (menu == 0)
    menu = 1;
  else
    menu = 0;
}

void updateEncoder() 
{
  int MSB = digitalRead(clk); 
  int LSB = digitalRead(dt); 
  int encoded = (MSB << 1) | LSB; 
  int sum  = (lastEncoded << 2) | encoded; 

  if (menu == 0)
  {
    if (sum == 13 || sum == 4 || sum == 2 || sum == 11)
      encoderValue += 1 * precisionScale;
    if (encoderValue > 12500000 * 4)
      encoderValue = 12500000 * 4;
    if (sum == 14 || sum == 7 || sum == 1 || sum == 8 ) {
      encoderValue -= 1 * precisionScale;
      if (encoderValue < 0 ) {
        encoderValue = 0;
      }
    }

  }
  else
  {
    if (sum == 13 || sum == 4 || sum == 2 || sum == 11)
    {
      if (precisionScale <= 1000000)
        precisionScale *= rootOfTen;
    }
    if (sum == 14 || sum == 7 || sum == 1 || sum == 8 )
    {
      if (precisionScale > 1) {
        precisionScale /= rootOfTen;
      }
    }

  }
  lastEncoded = encoded; 
}
