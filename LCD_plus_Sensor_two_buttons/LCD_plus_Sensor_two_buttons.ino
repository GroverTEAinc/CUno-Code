#include <SparkFunBME280.h>
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
 int i;

#define fwd_button 13 //blue button on LCD Module
#define bck_button 10 //yellow button on LCD Module

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Apply BME280 data to CCS811 for compensation.");

  //This begins the CCS811 sensor and prints error status of .begin()
  CCS811Core::status returnCode = myCCS811.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println("Problem with CCS811");
   // printDriverError(returnCode);
  }
  else
  {
    Serial.println("CCS811 online");
  }

  //Initialize BME280
  //For I2C, enable the following and disable the SPI section
  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x77;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  byte id = myBME280.begin(); //Returns ID of 0x60 if successful
  if (id != 0x60)
  {
    Serial.println("Problem with BME280");
  }
  else
  {
    Serial.println("BME280 online");
  }


  
  pinMode(fwd_button,INPUT);
  pinMode(bck_button,INPUT);
  lcd.begin(16,2);
  i=0;
  
}

void loop() {
  
  
  
  
  while(i==0) //Temperature Loop
  {
    
    
    
    
    
    temperature(); //call temperature function
      
      
      
      
      
      if( digitalRead(fwd_button) ==HIGH) //if blue button pushed, cycle to next loop
      {
        lcd.clear();
        delay(100);
        i++;
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear();
        delay(100);
        i=3;
      }
    
    
    
    delay(2);
  }

 
  
  
  
  
  
  
  
  while(i==1) //Humidity Loop
  {
    
    
    
    humid(); //call humid loop
      
      
      
      
      if( digitalRead(fwd_button) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear();
        delay(100);
        i++;
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear();
        delay(100);
        i--;
      }
    
    
    
    delay(2);
  }
  
  
  
  
  
  while(i==2)//carbon dioxide loop
  {
   
    
    
    if(myCCS811.dataAvailable()) //check to see if CC2811 is ready to print data
    {
      myCCS811.readAlgorithmResults(); //calculate sensor values
    }


    
      carbon(); //call carbon function which dsipalys c02 data
      
      
      
      if( digitalRead(13) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear();
        delay(100);
        i++;
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear();
        delay(100);
        i--;
      }
    
    delay(2);
  }

  while(i==3)
  {
    if(myCCS811.dataAvailable()) //check to see if CC2811 is ready to print data
    {
      myCCS811.readAlgorithmResults(); //calculate sensor values
    }


    
      TVOC(); //call TVOC function which dsipalys TVOC data
      
      
      
      if( digitalRead(13) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear();
        delay(100);
        i=0;
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear();
        delay(100);
        i--;
      }
    
    delay(2);
  }
}


void temperature()
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Temperature:         ");
    lcd.setCursor(0,1);
    lcd.print(myBME280.readTempC());
    lcd.setCursor(6,1);
    lcd.print("Celsius");
}

void humid()
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Humidity:         ");
    lcd.setCursor(0,1);
    lcd.print(myBME280.readFloatHumidity());
    lcd.setCursor(2,1);
    lcd.print("%         ");
}

void TVOC()
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("TVOC:     "); //print header
    lcd.setCursor(0,1);
    lcd.print(myCCS811.getTVOC());      //print data
    lcd.setCursor(6,1);
    lcd.print("ppb             ");
}

void carbon()
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("eCO2:     "); //print header
    lcd.setCursor(0,1);
    lcd.print(myCCS811.getCO2());      //print data
    lcd.setCursor(5,1);
    lcd.print("ppm             ");
}










