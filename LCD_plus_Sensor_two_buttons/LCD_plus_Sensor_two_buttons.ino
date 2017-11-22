#include <SparkFunBME280.h>
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B //Default I2C Address

//Global sensor objects
CCS811 myCCS811(CCS811_ADDR);
BME280 myBME280;

#include <LiquidCrystal.h> //include LCD Library

LiquidCrystal lcd(12,11,5,4,3,2); //define pins for LCD
int i; //state variable

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
    Serial.println("Problem with CCS811"); //prints error state to console
   printDriverError(returnCode);
  }
  else
  {
    Serial.println("CCS811 online"); //if no error, display online to console
  }

  //Initialize BME280
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
    
  pinMode(fwd_button,INPUT); //sets both buttons as inputs
  pinMode(bck_button,INPUT);
  lcd.begin(16,2); //declares LCD size
  i=0; //sets condition state to 0
  startup();
}

void loop() {  
  while(i==0) //Temperature Loop
  {   
    temperature(); //call temperature function
      
      if( digitalRead(fwd_button) ==HIGH) //if blue button pushed, cycle to next loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i++; //advance i to next state
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i=3;  //advance i to last state
      }   
    delay(2);
  }
  
  while(i==1) //Humidity Loop
  {
    humid(); //call humid loop
    
      if( digitalRead(fwd_button) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i++; //advance i to next state
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i--; //advance i to previous state
      }
    delay(2);
  }

  while(i==2) //carbon dioxide Loop
  {
    if(myCCS811.dataAvailable()) //check to see if CC2811 is ready to print data
    {
      myCCS811.readAlgorithmResults(); //calculate sensor values
    }
    
    carbon(); //call carbon function which dsipalys c02 data
      
      if( digitalRead(13) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i++; //advances i to next state
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i--; //advance i to previous state
      }
    delay(2);
  }

  while(i==3) //TVOC Loop
  {
    if(myCCS811.dataAvailable()) //check to see if CC2811 is ready to print data
    {
      myCCS811.readAlgorithmResults(); //calculate sensor values
    }
      
    TVOC(); //call TVOC function which dsipalys TVOC data
 
      if( digitalRead(13) ==HIGH) //if blue button is pushed, cycle to next loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i=0; //advance i to first state
      }
      if( digitalRead(bck_button) == HIGH) //if yellow button is pushed, cycle to previous loop
      {
        lcd.clear(); //clear lcd
        delay(100);
        i--; //advance i to previous state
      }
    delay(2);
  }
  
} //end of void loop


//functions called
void temperature() //display temperature to LCD
{
    delay(100); 
    lcd.setCursor(0,0);
    lcd.print("Temperature:         ");
    lcd.setCursor(0,1);
    lcd.print(myBME280.readTempC()); //read and print temp in C
    lcd.setCursor(6,1);
    lcd.print("Celsius"); //print units
}

void humid() //displays temperature to LCD
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Humidity:         "); //print header
    lcd.setCursor(0,1);
    lcd.print(myBME280.readFloatHumidity()); //read and print humidity as a percentage
    lcd.setCursor(2,1);
    lcd.print("%         "); //print units
}

void TVOC() //displays TVOC to LCD
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("TVOC:     "); //print header
    lcd.setCursor(0,1);
    lcd.print(myCCS811.getTVOC());      //read and print TVOC as ppb
    lcd.setCursor(6,1);
    lcd.print("ppb             "); //print units
}

void carbon()
{
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("eCO2:     "); //print header
    lcd.setCursor(0,1);
    lcd.print(myCCS811.getCO2());      //read and print CO2 as ppm
    lcd.setCursor(5,1);
    lcd.print("ppm             "); //print units
}

void printDriverError( CCS811Core::status errorCode ) //error state
{
  switch ( errorCode )
  {
    case CCS811Core::SENSOR_SUCCESS:
      Serial.print("SUCCESS");
      break;
    case CCS811Core::SENSOR_ID_ERROR:
      Serial.print("ID_ERROR");
      break;
    case CCS811Core::SENSOR_I2C_ERROR:
      Serial.print("I2C_ERROR");
      break;
    case CCS811Core::SENSOR_INTERNAL_ERROR:
      Serial.print("INTERNAL_ERROR");
      break;
    case CCS811Core::SENSOR_GENERIC_ERROR:
      Serial.print("GENERIC_ERROR");
      break;
    default:
      Serial.print("Unspecified error.");
  }
}

void startup() //startup text
{
  lcd.setCursor(0,0);
  lcd.print("     Grover");
  lcd.setCursor(0,1);
  lcd.print("by T.E.A., Inc.");
  delay(3000);
  lcd.clear();
}






