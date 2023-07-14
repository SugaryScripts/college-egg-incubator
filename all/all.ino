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
  float tempF; // temperature in Fahrenheit
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  tempF = tempC * 9 / 5 + 32; // convert °C to °F

  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  ");  // separator between °C and °F
  Serial.print(tempF);    // print the temperature in °F
  Serial.println("°F");
  return tempC;
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
  text = String(tempC) + "°C";
  Stringf();
  lcd.print(text);
  // set cursor to first column, second row
  //lcd.setCursor(6,0);
  //lcd.print("°C");
}


// Light Dimmer
#include <RBDdimmer.h>
int PIN_ZERO_CROSS[] = {33, 35};
int PIN_ACD[] = {32, 34};
dimmerLamp ACD_LAMP(32,33);
dimmerLamp ACD_FAN(34,35);


int LIGHT_MIN_POWER  = 0;
int LIGHT_MAX_POWER  = 80;
int FAN_MIN_POWER  = 0;
int FAN_MAX_POWER  = 80;
int POWER_STEP  = 2;
int power  = 0;

void testDimmer(){/* function testDimmer */
  for(power=LIGHT_MIN_POWER;power<=LIGHT_MAX_POWER;power+=POWER_STEP){
    ACD_LAMP.setPower(power); // setPower(0-100%);
    ACD_FAN.setPower(power);
    delay(200);
  }
  for(power=LIGHT_MAX_POWER;power>=LIGHT_MIN_POWER;power-=POWER_STEP){
    ACD_LAMP.setPower(power);
    ACD_FAN.setPower(power);
    delay(200);
  }
}

void setDimmer(int power, )

void setup() {
  // initialize Temperature
  DS18B20.begin();    // initialize the DS18B20 sensor 

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  // initialize PWM Lamp & Fan
  ACD_LAMP.begin(NORMAL_MODE, ON);
  ACD_FAN.begin(NORMAL_MODE, ON);

  Serial.begin(9600); // initialize serial
}


void loop() {
  setLCD();
  

  updateSeconds();
}
