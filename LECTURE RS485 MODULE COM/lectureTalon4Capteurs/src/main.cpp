#include <Arduino.h>
#define DE_RE_PIN 1 // GPIO1 pour contrôler DE/RE du transceiver RS485

void setup()
{
  Serial.begin(2000000); // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);   // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW); // Mode réception initial
}
void receive_data()
{
  if (Serial0.available() >= 11)
  {
    byte header1 = Serial0.read();
    if (header1 == 0x35)
    { // Vérifiez l'identifiant
      byte header2 = Serial0.read();
      if (header2 == 0x40)
      {
        byte counter = Serial0.read();
        byte force_1_HB = Serial0.read();
        byte force_1_LB = Serial0.read();
        byte force_2_HB = Serial0.read();
        byte force_2_LB = Serial0.read();
        byte force_3_HB = Serial0.read();
        byte force_3_LB = Serial0.read();
        byte force_4_HB = Serial0.read();
        byte force_4_LB = Serial0.read();
        int force_1 = (force_1_HB << 8) | force_1_LB;
        int force_2 = (force_2_HB << 8) | force_2_LB;
        int force_3 = (force_3_HB << 8) | force_3_LB;
        int force_4 = (force_4_HB << 8) | force_4_LB;
        
        Serial.print("F1: ");
        Serial.print(force_1);
        Serial.print(" F2: ");
        Serial.print(force_2);
        Serial.print(" F3: ");
        Serial.print(force_3);
        Serial.print(" F4: ");
        Serial.print(force_4);
        Serial.print(" Count : ");
        Serial.println(counter);
      }
    }
  }
}
void loop()
{
  receive_data();
}
