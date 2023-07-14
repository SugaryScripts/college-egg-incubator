float funSegitiga(float x,float p_left,float p_mid, float p_right){
  float nilai = 0;
  if (x <= p_left || x >= p_right){             // outside
    nilai = 0;
  } else if(p_left < x && x <= p_mid){          // left side
    nilai = (x - p_left) / (p_mid - p_left);
  } else if(p_mid < x && x < p_right){          // right side
    nilai = (p_right - x) / (p_right - p_mid);
  }
  return nilai;
}
float funTrapezoidNaik(float p_left,float x,float p_right){
  float nilai = 0;
  if (x <= p_left){
    nilai = 0;
  }else if(p_left <= x && x <= p_right){
    nilai = (x-p_left)/(p_right-p_left);
  }else if(x >= p_right){
    nilai = 1;
  }
  return nilai;
}
float funTrapezoidTurun(float p_left,float x,float p_right){
  float nilai = 0;
  if (x <= p_left){
    nilai = 1;
  }else if(p_left <= x && x <= p_right){
    nilai = (p_right-x) / (p_right-p_left);
  }else if(x >= p_right){
    nilai = 0;
  }
  return nilai;
}


float agregasi_naik(float p_right,float p_left,float alfa){
  if(alfa == 0) return 0;
  else {
    float nilai = 0;
    nilai = alfa*(p_right-p_left) + p_left;
    return nilai;
  }
}
float agregasi_turun(float p_right,float p_left,float alfa){
  if(alfa == 0) return 0;
  else {
    float nilai = 0;
    nilai = p_right - (alfa*(p_right-p_left));
    return nilai;
  }
}

// for fan 0 only
enum temperature_state {d, h, pa}; // dingin, hangat, panas
enum delta_temperature_state {n, s, p}; // negative, stable, positive
temperature_state rulesTemp[] = {d,d,d, h,h,h, pa,pa,pa};
delta_temperature_state rulesDelta[] = {n, p, s,n, p, s,n, p, s};
float agregassi_off_fan(int i){
  float data;
  if (rulesTemp[i] == pa && rulesDelta[i] == s){ // 9
    data = 0;
  } else if (rulesTemp[i] == pa &&  rulesDelta[i] == p){ // 8
    data = 0;
  } else if (rulesTemp[i] == pa &&  rulesDelta[i] == n){ // 7
    data = 0;
  } else if (rulesTemp[i] == h &&  rulesDelta[i] == s ){ // 6
    data = 1;
  } else if (rulesTemp[i] == h &&  rulesDelta[i] == p){ //5
    data = 0; 
  } else if (rulesTemp[i] == h &&  rulesDelta[i] == n){  //4
    data = 1;
  } else if (rulesTemp[i] == d &&  rulesDelta[i] == s){   //3
    data = 1;
  } else if (rulesTemp[i] == d &&  rulesDelta[i] == p){  //2
    data = 1;
  }else if (rulesTemp[i] == d &&  rulesDelta[i] == n) {   //1
    data = 1;
  }else {
    Serial.println("not found z fan rule");
    data = 3;
  }
  return data;
}



// https://www.anakkendali.com/tutorial-arduino-fuzzy-logic-controller-sugeno/
float temperature, deltaT;

// check current state iteration
const bool isFirst = 0;

const float ex = 37;

float in_rule_suhu[3] = {36, 38, 40};     // D H P
float in_rule_delta_suhu[3] = {-5, 0, 5}; // N S P

float out_rule_lamp[4] = {20,40,60,80};   // SR R T ST
float out_rule_fan[4] = {0, 30, 60, 90};  // 0 L M H

enum lamp_state {SR, R, T, ST};
enum fan_state {O, L, M, H};
lamp_state rulesLamp[] = {ST, T, ST, T, R, R, R, SR, R};
fan_state rulesFan[] = {O, O, O, O, M, O, L, H, H};
const int RULES_LEN = 9;


float prediction_suhu[9];
float prediction_delta_suhu[9];

float alphas[9];
float zeeLamp[9];
float zeeFan[9];

void fuzzy_input(float suhu, float delta_suhu) {
  float temp_anggota_suhu[3];
  float temp_anggota_delta_suhu[3];
  //dingin
  temp_anggota_suhu[0] = funTrapezoidTurun(in_rule_suhu[0], suhu, in_rule_suhu[1]);
  //float hangat
  temp_anggota_suhu[1] = funSegitiga(suhu, in_rule_suhu[0], in_rule_suhu[1], in_rule_suhu[2]);
  //float panas
  temp_anggota_suhu[2] = funTrapezoidNaik(in_rule_suhu[1], suhu, in_rule_suhu[2]);

  //float negative
  temp_anggota_delta_suhu[0] = funTrapezoidTurun(in_rule_delta_suhu[0], delta_suhu, in_rule_delta_suhu[1]);
  //float positive
  temp_anggota_delta_suhu[1] = funTrapezoidNaik(in_rule_delta_suhu[1], delta_suhu, in_rule_delta_suhu[2]);
  //float stable
  temp_anggota_delta_suhu[2] = funSegitiga(delta_suhu, in_rule_delta_suhu[0], in_rule_delta_suhu[1], in_rule_delta_suhu[2]);

  //dingin 1, hangat 0.5, panas=0

  int k = 0;
  for (int i = 0; i < 9; i++){
    int j = round(i/3);
    prediction_suhu[i] = temp_anggota_suhu[j];
    //Serial.printf("i %i j %i k %i \n", i,j,k);
    
    prediction_delta_suhu[i] = temp_anggota_delta_suhu[k];
    k++;
    if (k>=3) k = 0;
  }

  for (int i = 0; i < 9; i++){
    alphas[i] = min(prediction_suhu[i], prediction_delta_suhu[i]);
    Serial.printf("%i. suhu %f : dsuhu %f = %f \n",i, prediction_suhu[i], prediction_delta_suhu[i], alphas[i]);
  }

}

void calculateZeeFan(float suhu, float delta_suhu){
  float z, zl, zr;
  for (int i = 0; i < 9; i++){
    z=0; zl=0; zr=0;
    switch (rulesFan[i]){
      case O:
        z = agregassi_off_fan(i);
        Serial.printf("O left=%f right=%f z=%f \n", zl, zr, z);
      break;
      case L:
        zl = agregasi_naik(out_rule_fan[1], out_rule_fan[0], alphas[i]);
        zr = agregasi_turun(out_rule_fan[2], out_rule_fan[1], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("L left=%f right=%f z=%f \n", zl, zr, z);
      break;
      case M:
        zl = agregasi_naik(out_rule_fan[2], out_rule_fan[1], alphas[i]);
        zr = agregasi_turun(out_rule_fan[3], out_rule_fan[2], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("M left=%f right=%f z=%f \n", zl, zr, z);
      break;
      case H:
        zl = agregasi_naik(out_rule_fan[3], out_rule_fan[2], alphas[i]);
        zr = 1; // tidak terpakai krn graph kipas 60 - 90 itu; Jadi, mskpn input >= 90 ttp saja output 90, krn bkn garis miring
        z = zl;
        Serial.printf("H left=%f right=%f z=%f \n", zl, zr, z);
      break;
    }
    zeeFan[i] = z;
  }
}

void calculateZeeLamp() {
  float z, zl, zr;
  for (int i = 0; i < 9; i++){
    z=0; zl=0; zr=0;
    switch (rulesLamp[i]){
      case SR:
        zl = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        zr = agregasi_turun(out_rule_lamp[2], out_rule_lamp[1], alphas[i]);;
        z = zr;
        Serial.printf("SR alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case R:
        zl = agregasi_naik(out_rule_lamp[1], out_rule_lamp[0], alphas[i]);
        zr = agregasi_turun(out_rule_lamp[2], out_rule_lamp[1], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("R alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case T:
        zl = agregasi_naik(out_rule_lamp[2], out_rule_lamp[1], alphas[i]);
        zr = agregasi_turun(out_rule_lamp[3], out_rule_lamp[2], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("T alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case ST:
        zl = agregasi_naik(out_rule_lamp[3], out_rule_lamp[2], alphas[i]);
        zr = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        z = zl;
        Serial.printf("ST alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
    }
    zeeLamp[i] = z;
  }
}

void calculateResultLamp(){
  float top = 0;
  float bot = 0;
  float result = 0;

  for (int i = 0; i < 9; i++){
    top += alphas[i] * zeeLamp[i];
    bot += alphas[i];
  }
  Serial.printf("%f / %f \n", top, bot);
  result = top / bot;
  Serial.println(result);
}

void calculateResultFan(){
  float top = 0;
  float bot = 0;
  float result = 0;

  for (int i = 0; i < 9; i++){
      top += alphas[i] * zeeFan[i];
      bot += alphas[i];
    }
  Serial.printf("%f / %f \n", top, bot);
  result = top / bot;

  Serial.println(result);
}


void setup() {
  Serial.begin(9600);
}

void loop() {

  fuzzy_input(39, 2);
  Serial.println("==== Fuzzy Lampu ====");
  calculateZee();
  calculateResultLamp();
  Serial.println("==== Fuzzy Fan ====");
  calculateZeeFan(39, 2);
  calculateResultFan();
  
  Serial.println();
  delay(5000);
}