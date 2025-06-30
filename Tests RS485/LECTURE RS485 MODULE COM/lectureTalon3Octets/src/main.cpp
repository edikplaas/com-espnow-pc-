
#include <Arduino.h>
#define DE_RE_PIN 1      // GPIO1 pour contrôler DE/RE du transceiver RS485

void setup() {
  Serial.begin(2000000);  // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);  // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW);  // Mode réception initial
}
void receive_data() {
  if (Serial0.available() >= 2) {
  byte headers[2];
  Serial0.readBytes(headers,2);
  if (headers[0] == 0x01 && headers[1] == 0x02) { // Vérifiez l'identifiant
    byte receivedByte1 = Serial0.read();
    byte receivedByte2 = Serial0.read();
    Serial.print(" Data: ");
    Serial.print(receivedByte1);
    Serial.print(" ");
    Serial.println(receivedByte2);
  }
}



}
void loop() {
  receive_data();
}


