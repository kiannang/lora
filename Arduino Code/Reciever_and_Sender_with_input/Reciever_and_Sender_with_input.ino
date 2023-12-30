#include <SPI.h>
#include <LoRa.h>

String message = "";
int counter = 0;

void setup() {
  Serial.begin(9600); //bautrate
  Serial.println("LoRa Reciever/Sender");
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  delay(1000);
}

void loop() {


  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    message += c;
  }

  if (message.length() > 0) {
    Serial.print("Sending Packet: "); //seen in Serial Monitor
    Serial.println(counter);
    LoRa.beginPacket();
    LoRa.print("Received Packet: "); //sent to package
    LoRa.print(counter);
    LoRa.endPacket();
    message = "";
    counter++;
  }

  onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
  if (packetSize == 0) return; 

  int recipient = LoRa.read();
  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  Serial.print(incoming);
  Serial.print(" || RSSI: ");
  Serial.println(LoRa.packetRssi());
  Serial.println();
}

