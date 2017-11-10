void setup() {
  i = 0;
}

void loop() {
  while(i==0)  //Temp Loop
  {
    read/display temperature data

      if(button is pushed)
      {
        i+=1;
      }
    delay(250);
  }

  while(i==1)  //Humid Loop
  {
    read/display Humidity Data    
      if(button is pushed)
      {
        i+=1;
      }
    delay(250);
  }

  while(i==2)  //CO2 Loop
  {
    read/display CO2 data    
      if(button is pushed)
      {
        i=0;  //Reset to Temp Loop
      }
    delay(250);
  }

}
