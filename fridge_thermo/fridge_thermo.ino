#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin 10 on the Arduino
#define ONE_WIRE_BUS 10


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
boolean o1;



// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/*
ROM = 28 E3 34 7F 4 0 0 59
  Chip = DS18B20
  Data = 1 C9 1 4B 46 7F FF 7 10 64  CRC=64
  Temperature = 28.56 Celsius, 83.41 Fahrenheit
ROM = 28 F A1 EB 3 0 0 FD
  Chip = DS18B20
  Data = 1 C8 1 4B 46 7F FF 8 10 3F  CRC=3F
  Temperature = 28.50 Celsius, 83.30 Fahrenheit
No more addresses.
*/

  DeviceAddress firstThermometer = {  
    0x28, 0xE3, 0x34, 0x7F, 0x04, 0x00, 0x00, 0x59 };
  
  DeviceAddress secondThermometer = {
    0x28, 0x0F, 0xA1, 0xEB, 0x03, 0x00, 0x00, 0xFD };
  int fridgeControlPin = 8;
  
  
  long previousMillis = 0;
  long interval = 20;           //number of seconds between readings
  long previousMillisStore = 0;
  long intervalStore = 90;      //store every 15 minutes : 15*60 = 900
  float FridgeHighSP = 2.5E1;  //this is the temperature at which we turn on the fridge
  float FridgeLowSP = 1.9E1;   //this is the temperature at which we turn off the fridge
  float temp1;
  const int NumberQuantity = 10;
  float tempLog[NumberQuantity];
  
  
  void setup()
{
  Serial.begin(9600);
  //sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(firstThermometer, 10);
  sensors.setResolution(secondThermometer, 10);
  
  Serial.println("thermometer initialized");

    // initialize the digital pin as an output.
  pinMode(fridgeControlPin, OUTPUT);  
  
  //tempLog[1] = 5.6;
}


void loop()
{
//String dataString;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval*1000) {
    // save the last time you updated saved and sent data
    previousMillis = currentMillis;   

    Serial.print("Getting temperatures...\n\r");
    sensors.requestTemperatures();

    // make a string for assembling the data to log:
  ///  dataString = String(previousMillis/1000) + ",";

    //FIRST THERMOMETER
    int tempC_dec = 100*sensors.getTempC(firstThermometer);
    int tempC_int = sensors.getTempC(firstThermometer);
    String tempString = String(tempC_int)+"."+String(tempC_dec-100*tempC_int);
    Serial.println("Temperature 1= "+tempString);
    temp1 = sensors.getTempC(firstThermometer);
    //Serial.println("T1 float = "+temp1);
    
    
    
    //SECOND THERMOMETER
    tempC_dec = 100*sensors.getTempC(secondThermometer);
    tempC_int = sensors.getTempC(secondThermometer);
    tempString = String(tempC_int)+"."+String(tempC_dec-100*tempC_int);
    Serial.println("Temperature 2= "+tempString);
    //float temp2 = sensors.getTempC(secondThermometer); 
 
 
     //control logic. 
    //---------------------------------------------
    //
    //  TEMP1 is the sensor that goes inside the fridge
    //
    //
    //--------------------------------------------
    if (temp1>FridgeHighSP){
      digitalWrite(fridgeControlPin, HIGH);
      o1 =1;
    } else if (temp1<FridgeLowSP){
      digitalWrite(fridgeControlPin, LOW);
      o1=0;
    }
    
    if (o1){
      Serial.println("The fridge is ON");
    }else{
      Serial.println("The fridge is OFF");
    }
    
    //for(int i = 5; i == 0; i--){
    //    tempLog[i] = tempLog[i-1];
    // }
    //tempLog[0] = temp1;
  
  }
    
      if(currentMillis - previousMillisStore > intervalStore*1000) {
    // save the last time you updated saved and sent data
    previousMillisStore = currentMillis;   
     //tempLog[10]=tempLog[9];
     tempLog[9]=tempLog[8];
     tempLog[8]=tempLog[7];
     tempLog[7]=tempLog[6];
     tempLog[6]=tempLog[5];
     tempLog[5]=tempLog[4];
     tempLog[4]=tempLog[3];
     tempLog[3]=tempLog[2];
     tempLog[2]=tempLog[1];
     tempLog[1]=tempLog[0];
     tempLog[0]=temp1;
     //Serial.println("The last 10 (15 minute blocks) temperatures are: ");
    //for(int i = 0; i < NumberQuantity-1; i++){
    //  Serial.print(tempLog[i]);
    //  Serial.print(",");
    //}
    // Serial.println();
    }  
    //Serial.write("The fridge is turned :",  
    
    
  
}



