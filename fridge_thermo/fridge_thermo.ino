#include <OneWire.h>
#include <DallasTemperature.h>
#include <DFR_Key.h>
#include <LiquidCrystal.h>

// Data wire is plugged into port x on the Arduino
#define ONE_WIRE_BUS 3
//#define DEBUG true
#define FRIDGE_CTRL_PIN 12

// Uncomment this line if you are using the updated dallas_temp_library that 
// supports the busFail() method to diagnose bus problems
// #define BUSFAIL 

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DFR_Key keypad;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int localKey = 0;
int sp_h =20;
int sp_l = 0;

bool cooling = false;

// initialize the library with the numbers of the interface pins


void setup()
{

  // This lines just make it so you can plug a DS18B20 directly into 
  // digitial pins 8-10. 
  
  digitalWrite( ONE_WIRE_BUS-1 , LOW );
  pinMode( ONE_WIRE_BUS-1  , OUTPUT );
  digitalWrite( ONE_WIRE_BUS+1 , LOW );  
  pinMode( ONE_WIRE_BUS+1 , OUTPUT );

  //Setup Fridge output pin and neighbor as ground to allow direct connection
  pinMode( FRIDGE_CTRL_PIN , OUTPUT);
  digitalWrite (FRIDGE_CTRL_PIN, LOW);
  pinMode( FRIDGE_CTRL_PIN+1, OUTPUT);
  digitalWrite(FRIDGE_CTRL_PIN+1, LOW);
   // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Beer controller");
  keypad.setRate(10);
  delay(1000);

}

void loop(){
  //load the temperature from the DS18B20 (only one on the network at this stage)
  DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address    
  // For testing purposes, reset the bus every loop so we can see if any devices appear or fall off
  sensors.begin();
  sensors.requestTemperatures(); // Send the command to get temperatures
  sensors.getAddress(tempDeviceAddress, 1);
  float tempC = sensors.getTempC(tempDeviceAddress);
  //Serial.println(tempC); // Converts tempC to Fahrenheit
  int tempI = int(tempC);
  Serial.println(tempI);

  //Handle changes to the setpoint
  localKey = keypad.getKey();
  if (localKey != SAMPLE_WAIT) {
    //put the code here to change the setpoint
    if (localKey == 3) sp_h ++;
    if (localKey == 4) sp_h --;
    sp_l = sp_h - 2;
    delay(300);
  }  



  //Control the fridge
  if (tempI >= sp_h){
      digitalWrite(FRIDGE_CTRL_PIN, HIGH);
      cooling = true;
     } else if (tempI<sp_l){
      digitalWrite(FRIDGE_CTRL_PIN, LOW);
      cooling = false;
    }

      //Print the display contents
  //clear the LCD
  lcd.clear();
  lcd.print("Beer Controller");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("Temp:");
  //lcd.setCursor(6, 1);
  lcd.print(tempI);
  lcd.setCursor(9, 1);
  lcd.print("SP:");
  lcd.print(sp_h);
  lcd.setCursor(15, 1);
  if (cooling == HIGH) lcd.print("+");
  else lcd.print("_");
}

