#include <SPI.h>
#include <LoRa.h>

double longitude = 37.366014123574836;
double latitude = -120.42156523566737;
int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
//send counter also
void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Received Packet: ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}