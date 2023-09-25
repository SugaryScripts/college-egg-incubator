int LDR_Val = 0;     /* Variabel untuk menyimpan nilai photoresistor */
int ldrPin = 15;      /* Input Analog untuk fotoresistor */
const float gama = 0.7;
const float rl10 = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
  int nilaiLDR = analogRead(ldrPin);
  nilaiLDR = map(nilaiLDR, 4095, 0, 1024, 0); //mengubah nilai pembacaan sensor LDR dari nilai ADC arduino menjadi nilai ADC ESP32
  float voltase = nilaiLDR / 1024.*5;
  float resistansi = 2000 * voltase / (1-voltase/5);
  float kecerahan = pow(rl10*1e3*pow(10,gama)/resistansi,(1/gama));
  Serial.print("Kecerahan = ");
  Serial.println(kecerahan);
  delay(500);
}