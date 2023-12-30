#include <SPI.h>
#include <LoRa.h>

//String message = "";
int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Reciever/Sender");
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  delay(1000);
}

void loop() {

  for(int i = 0; i < 10; i++){
    if(LoRa.parsePacket() != 0){
      onReceive(LoRa.parsePacket());
    }
    delay(1000);
  }

  Serial.print("Sending Packet: "); //seen in Serial Monitor
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print("Received Packet: "); //sent to package
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;

  delay(5000);
}

void onReceive(int packetSize) {
  int recipient = LoRa.read();
  String incoming = "";

  while (LoRa.available()) {
    incoming = incoming + (char)LoRa.read();
  }

  Serial.print(incoming);
  Serial.print(" || RSSI: ");
  Serial.println(LoRa.packetRssi());
  Serial.println();
}

