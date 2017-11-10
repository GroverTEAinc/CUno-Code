#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

int i, temp, hum, co2;

void setup() {
  pinMode(13,INPUT);
  lcd.begin(16,2);
  
  i=0;
  temp = 23;
  hum = 65;
  co2= 450;
}

void loop() {
  while(i==0)
  {
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Temperature:         ");
    lcd.setCursor(0,1);
    lcd.print(temp);
    lcd.setCursor(4,1);
    lcd.print("Celsius");
      if( digitalRead(13) ==HIGH)
      {
        lcd.clear();
        i++;
      }
    delay(2);
  }

  while(i==1)
  {
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Humidity:         ");
    lcd.setCursor(0,1);
    lcd.print(hum);
    lcd.setCursor(3,1);
    lcd.print("%         ");
      if( digitalRead(13) ==HIGH)
      {
        lcd.clear();
        i++;
      }
    delay(2);
  }
  
  while(i==2)
  {
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Carbon Dioxide:     ");
    lcd.setCursor(0,1);
    lcd.print(co2);
    lcd.setCursor(4,1);
    lcd.print("ppm             ");
      if( digitalRead(13) ==HIGH)
      {
        lcd.clear();
        i=0;
      }
    delay(2);
  }
}
