double divVal = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void print_1() {
  Serial.print(10 + cos(divVal));
}




\


void print_2() {
  Serial.print(20 + sin(divVal));
}

void print_3() {
  Serial.print(30 + cos(divVal + PI));  
}

void loop() {
  // put your main code here, to run repeatedly:
  print_1();
  Serial.print(",");
  print_2();
  Serial.print(",");
  print_3();
  Serial.print("\n");
  divVal +=  2 * PI / 16.0;
  delay(100);
}
