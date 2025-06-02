#include <Arduino.h>
#define DE_RE_PIN 1      // GPIO1 pour contrôler DE/RE du transceiver RS485

void setup() {
  Serial.begin(2000000);  // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);  // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW);  // Mode réception initial
}
void receive_data() {
  if (Serial0.available()>0) {    // Vérifie si des données sont disponibles
    int receivedByte = Serial0.read();  // Lecture d'un octet reçu
    Serial.print("Received data: ");
    Serial.println(receivedByte);
  }

}
void loop() {
  receive_data();
}




