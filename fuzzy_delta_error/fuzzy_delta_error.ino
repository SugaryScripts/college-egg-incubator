// ============== fuzzy helper ==============
float funTrapezoidNaik(float p_left,float x,float p_right){
  float nilai = 0;
  if (x <= p_left){
    nilai = 0;
  }else if(x > p_left && x < p_right){
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
  }else if(x > p_left && x < p_right){
    nilai = (p_right-x) / (p_right-p_left);
  }else if(x >= p_right){
    nilai = 0;
  }
  return nilai;
}
float funSegitiga(float x,float p_left,float p_mid, float p_right){
  float nilai = 0;
  if (x == p_mid) {
    nilai = 1;
  } else if(x < p_mid){          // left side
    nilai = funTrapezoidNaik(p_left, x, p_mid);
  } else if(x > p_mid){          // right side
    nilai = funTrapezoidTurun(p_mid, x, p_right);
  }
  return nilai;
} 
float agregasi_naik(float p_left,float p_right,float alfa){
  if(alfa == 0) return 0;
  else {
    float nilai = 0;
    nilai = alfa*(p_right-p_left) + p_left;
    return nilai;
  }
}
float agregasi_turun(float p_left,float p_right,float alfa){
  if(alfa == 0) return 0;
  else {
    float nilai = 0;
    nilai = p_right - (alfa*(p_right-p_left));
    return nilai;
  }
}

// ============== fuzzy variables ==============

enum err_state {nbig, nsmall, psmall, pbig};
enum delta_err_state {n, s, p};
enum lamp_state {R,C,T};    //redup, cukup, terang
enum fan_state {L,M,H};     // lo, mid, hi

float graph_err[4] = {-10, -5, 5, 10};
float graph_delta_err[3] = {-2, 0, 2};
float graph_lamp[3] = {20, 50, 80};
float graph_fan[3] = {0, 40, 80};

const int RULE_LEN = 12;
err_state rule_err[RULE_LEN] = {nbig, nbig, nbig, nsmall, nsmall, nsmall, psmall, psmall, psmall, pbig, pbig, pbig};
delta_err_state rule_delta_err[RULE_LEN] = {n,s,p,n,s,p,n,s,p,n,s,p};
lamp_state rule_lamp[RULE_LEN] = {T,T,T,T,T,C,C,C,C,R,R,R};
fan_state rule_fan[RULE_LEN] = {L,L,L,L,L,L,L,L,M,M,M,H};

float prediction_error[RULE_LEN];
float prediction_delta_error[RULE_LEN];
float alphas[RULE_LEN];

float zeeLamp[RULE_LEN];
float zeeFan[RULE_LEN];

// ============== start fuzzy calculate function ==============
void fuzzyfication(float error, float delta_error) {
  float temp_anggota_error[4];
  float temp_anggota_delta_error[3];

  temp_anggota_error[0] = funTrapezoidTurun(graph_err[0], error, graph_err[1]);
  temp_anggota_error[1] = funSegitiga(error, graph_err[0], graph_err[1], graph_err[2]);
  temp_anggota_error[2] = funSegitiga(error, graph_err[1], graph_err[2], graph_err[3]);
  temp_anggota_error[3] = funTrapezoidNaik(graph_err[2], error, graph_err[3]);

  temp_anggota_delta_error[0] = funTrapezoidTurun(graph_delta_err[0], delta_error, graph_delta_err[1]);
  temp_anggota_delta_error[1] = funSegitiga(delta_error, graph_delta_err[0], graph_delta_err[1], graph_delta_err[2]);
  temp_anggota_delta_error[2] = funTrapezoidNaik(graph_delta_err[1], delta_error, graph_delta_err[2]);

  int k = 0;
  for (int i = 0; i < RULE_LEN; i++){
    int j = round(i/3);
    prediction_error[i] = temp_anggota_error[j];
    //Serial.printf("i %i j %i k %i \n", i,j,k);
    
    prediction_delta_error[i] = temp_anggota_delta_error[k];
    k++;
    if (k>=3) k = 0;
  }

  for (int i = 0; i < RULE_LEN; i++){
    alphas[i] = min(prediction_error[i], prediction_delta_error[i]);
    Serial.printf("%i. error %f : d_error %f = %f \n",i, prediction_error[i], prediction_delta_error[i], alphas[i]);
  }

}
void defuzzLamp(){
  Serial.println("==== Zee Lamp ====");float z=0, zl=0, zr=0;
  for (int i = 0; i < 9; i++){
    switch (rule_lamp[i]){
      case R:
        zl = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        zr = agregasi_turun(graph_lamp[0], graph_lamp[1], alphas[i]);;
        z = zr;
        Serial.printf("redup alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case C:
        zl = agregasi_naik(graph_lamp[0], graph_lamp[1], alphas[i]);
        zr = agregasi_turun(graph_lamp[1], graph_lamp[2], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("cukup alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case T:
        zl = agregasi_naik(graph_lamp[1], graph_lamp[2], alphas[i]);
        zr = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        z = zl;
        Serial.printf("terang alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
    }
    zeeLamp[i] = z;
  }
}
void defuzzFan(){
  Serial.println("==== Zee Fan ===="); float z=0, zl=0, zr=0;
  for (int i = 0; i < 9; i++){
    switch (rule_fan[i]){
      case L:
        zl = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        zr = agregasi_turun(graph_fan[0], graph_fan[1], alphas[i]);;
        z = zr;
        Serial.printf("Low alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case M:
        zl = agregasi_naik(graph_fan[0], graph_fan[1], alphas[i]);
        zr = agregasi_turun(graph_fan[1], graph_fan[2], alphas[i]);
        z = (zr+zl) / 2;
        Serial.printf("Mid alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
      case H:
        zl = agregasi_naik(graph_fan[1], graph_fan[2], alphas[i]);
        zr = 1; // tidak terpakai krn graph lamp 60 - 80 itu; Jadi, mskpn input >= 90 ttp saja output 80, krn bkn garis miring
        z = zl;
        Serial.printf("High alfa=%f left=%f right=%f z=%f \n", alphas[i],zl, zr, z);
      break;
    }
    zeeFan[i] = z;
  }
}
void resultLamp(){
  Serial.println("==== Result Lamp ====");float top = 0, bot = 0, result = 0;

  for (int i = 0; i < 9; i++){
    top += alphas[i] * zeeLamp[i];
    bot += alphas[i];
  }
  Serial.printf("%f / %f \n", top, bot);
  result = top / bot;
  Serial.println(result);
}
void resultFan(){
  Serial.println("==== Result Fan ====");float top = 0, bot = 0, result = 0;

  for (int i = 0; i < 9; i++){
    top += alphas[i] * zeeFan[i];
    bot += alphas[i];
  }
  Serial.printf("%f / %f \n", top, bot);
  result = top / bot;
  Serial.println(result);
}
// ============== end fuzzy calculate function ==============

// ============== start fuzzy main ==============
float e = 0, delta_e = 0;
const float setpoint = 37;
bool isFirst = true;
void fuzzy(float temperature) {
  Serial.println("===== Start Fuzzy =====");
  
  float e_was = e;
  e = temperature - setpoint;
  
  if (isFirst) isFirst = false; else delta_e = e - e_was;

  Serial.printf("e1 %f, e2 %f, delta_e %f \n", e_was, e, delta_e);

  fuzzyfication(e, delta_e);
  defuzzLamp();
  defuzzFan();
  resultLamp();
  resultFan();

  Serial.println("===== End Fuzzy =====");
}
// ============== end fuzzy main ==============

void setup() {
  Serial.begin(9600);

  
}

void loop() {
  e = 0; delta_e = 1;isFirst = true;
  delay(5000);
  fuzzy(28);
  //fuzzy(27);
  Serial.println("\n\n\n\n\n");
}
