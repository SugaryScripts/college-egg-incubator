int seconds = 0;
void updateSeconds(){
  if (seconds > 240000 - 1000) {seconds = 0;} // 4 minute
  else {seconds += 1000;} delay(1000);
}

// Sensor Temperature
#include <OneWire.h>
#include <DallasTemperature.h>
#define PIN_SENSOR_TEMPERATURE  17 // ESP32 pin GIOP17 connected to DS18B20 sensor's DATA pin
OneWire oneWire(PIN_SENSOR_TEMPERATURE);
DallasTemperature DS18B20(&oneWire);
float getTemperature() {
  float tempC; // temperature in Celsius
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  return tempC;
}
String getTemperatureText(){
  float tempC = getTemperature(); // temperature in Celsius
  float tempF; // temperature in Fahrenheit
  tempF = tempC * 9 / 5 + 32; // convert °C to °F
  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  ");  // separator between °C and °F
  Serial.print(tempF);    // print the temperature in °F
  Serial.println("°F");
  return "Temperature: " + 1;
}



#define PIN_SDA_LCD 21
#define PIN_SCL_LCD 22
#include <LiquidCrystal_I2C.h>
// set the LCD number of columns and rows
const int lcdColumns = 16, lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setLCD() {
  lcd.clear(); 

  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  String text;
  text = String(getTemperature()) + "°C";
  //Stringf("%f C",t);
  lcd.print(text);
  // set cursor to first column, second row
  //lcd.setCursor(6,0);
  //lcd.print("°C");
}


// Light Dimmer
//#include <RBDdimmer.h>
#include "RBDdimmer1.h"
#include "RBDdimmer2.h"
dimmerLamp1 ACD_LAMP(19,23); // acd, zero cross
dimmerLamp2 ACD_FAN(25,26);//26

// dimmerLamp dimmer[] = {
//   ACD_LAMP(32, 32),
//   ACD_FAN(33, 32)  
// };


int LIGHT_MIN_POWER  = 0;
int LIGHT_MAX_POWER  = 120;
int FAN_MIN_POWER  = 0;
int FAN_MAX_POWER  = 120;
int POWER_STEP  = 60;
int power  = 60;

void testDimmer(){/* function testDimmer */
  for(power=LIGHT_MIN_POWER;power<=LIGHT_MAX_POWER;power+=POWER_STEP){
    //ACD_LAMP.setPower(power); // setPower(0-100%);
    //ACD_FAN.setPower(power);
    delay(10000);
  }
  for(power=LIGHT_MAX_POWER;power>=LIGHT_MIN_POWER;power-=POWER_STEP){
    //ACD_LAMP.setPower(power);
    //ACD_FAN.setPower(power);
    delay(10000);
  }
}

volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;
void rpm_fun(){
  rpmcount++;
}


void setup() {
  // IR Infrared sensor
pinMode(12, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(12), rpm_fun, FALLING);
  rpmcount = 0;
  rpm = 0;
  timeold = 0;

  // initialize Temperature
  DS18B20.begin();    // initialize the DS18B20 sensor 

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // initialize PWM Lamp & Fan
  ACD_LAMP.begin1(0, true);
  ACD_FAN.begin2(0, true);

  Serial.begin(9600); // initialize serial

  ACD_LAMP.setPower1(80);
  ACD_FAN.setPower2(80);
}

void loop() {
  setLCD();

//   detachInterrupt(0);
//   rpm = 30*1000/(millis() - timeold)*rpmcount;
//   timeold = millis();
//   rpmcount = 0;
// power = 80;
//   ACD_LAMP.setPower(80); // setPower(0-100%);
//    ACD_FAN.setPower(80);
//    delay(1000);
    //testDimmer();

  ACD_LAMP.setPower1(80);
  ACD_FAN.setPower2(80);

  delay(3000);


  ACD_LAMP.setPower1(30);
  ACD_FAN.setPower2(0);

Serial.println("hello");
  delay(3000);

    
//   power += 60;
//   if (seconds % 10 == 0) {
//     if (power >= 180 ) {power = 0;}
//   }
  
//   int sensorValue = analogRead(34);
//   // print out the value you read:
//   Serial.print("cahaya ");
//   Serial.println(sensorValue);
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("Kecepatan");
//   lcd.setCursor(0,1);
//   lcd.print(rpm);

//   lcd.print(" rpm");

//    //Restart the interrupt processing

//   attachInterrupt(0, rpm_fun, FALLING);


//   updateSeconds();
}
