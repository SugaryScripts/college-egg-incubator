//Libraries
#include <RBDdimmer.h>//https://github.com/RobotDynOfficial/RBDDimmer
//Parameters
const int zeroCrossPin  = 23;
const int acdPin  = 25;
int MIN_POWER  = 0;
int MAX_POWER  = 80;
int POWER_STEP  = 2;
//Variables
int power  = 0;
//Objects
dimmerLamp acd(acdPin,zeroCrossPin);
dimmerLamp ACD_LAMP(22,23);
void setup(){
  //Init Serial USB
  Serial.begin(115200);
  Serial.println("ESP32 System");
  acd.begin(NORMAL_MODE, ON);
  ACD_LAMP.begin(NORMAL_MODE, ON);
  acd.setPower(80);
  ACD_LAMP.setPower(80);
}
void loop(){
  //testDimmer();
  
  Serial.println(acd.getPower());
  delay(1000);
}

void testDimmer(){/* function testDimmer */
////Sweep light power to test dimmer
  for(power=MIN_POWER;power<=MAX_POWER;power+=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
    delay(100);
  }
  for(power=MAX_POWER;power>=MIN_POWER;power-=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
    delay(100);
  }
}