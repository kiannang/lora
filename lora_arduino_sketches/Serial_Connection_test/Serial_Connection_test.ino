void setup() {
  // put your setup code here, to run once:
  //begin the transmission and reception
  Serial.begin(9600); //9600 is the bits per second
  pinMode(led, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //wait until some data is received
  If(Serial.available()){
    int data = Serial.read();
    if(data == 'A')
      digitalWrite(led, HIGH);
    else
      digitalWrite(led,LOW);
  }
}
