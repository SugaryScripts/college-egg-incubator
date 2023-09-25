#define IR_Sensor 4 /*D14 IR pin defined*/
#define LED 27       /*D27 LED Pin defined*/
int IR;             /*Variable that will store IR output status*/
void setup() {
  Serial.begin(9600);
  pinMode(IR_Sensor, INPUT);   /*IR Pin D14 defined as Input*/
  pinMode(LED, OUTPUT);        /*D27 pin for LED is set as Output*/
}
void loop(){
    IR=digitalRead(IR_Sensor);  /*digital read function to check IR pin status*/
    if(IR==LOW){               /*If sensor detect any reflected ray*/
    Serial.println("1");   /*LED will turn ON*/
  }
  else {
    digitalWrite(LED,LOW);  /*if no reflection detected LED will remain OFF*/
  Serial.println("0");
  }
}