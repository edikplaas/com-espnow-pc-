#include <Arduino.h>
#define DE_RE_PIN 1      // GPIO1 pour contrôler DE/RE du transceiver RS485

void setup() {
  Serial.begin(2000000);  // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);  // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW);  // Mode réception initial
}
void receive_data() {
  if (Serial0.available()>=4) {
  byte header = Serial0.read();
  if (header == 0x35) { // Vérifiez l'identifiant
    byte counter = Serial0.read();
    byte force_1_HB = Serial0.read();
    byte force_1_LB = Serial0.read();
    
    int force_1 = (force_1_HB << 8) | force_1_LB;
    Serial.print("Header:");
    Serial.print(header);
    Serial.print(" Counter: ");
    Serial.print(counter);
    Serial.print(" Force 1: ");
    Serial.println(force_1);
  }
}

}
void loop() {
  receive_data();
}








