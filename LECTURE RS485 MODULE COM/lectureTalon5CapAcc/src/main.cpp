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
  if (Serial0.available() >= 17)
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
        byte accX_HB = Serial0.read();
        byte accX_LB = Serial0.read();
        byte accY_HB = Serial0.read();
        byte accY_LB = Serial0.read();
        byte accZ_HB = Serial0.read();
        byte accZ_LB = Serial0.read();
        uint16_t force_1 = (force_1_HB << 8) | force_1_LB;
        uint16_t force_2 = (force_2_HB << 8) | force_2_LB;
        uint16_t force_3 = (force_3_HB << 8) | force_3_LB;
        uint16_t force_4 = (force_4_HB << 8) | force_4_LB;
        int16_t accX = (accX_HB << 8) | accX_LB;
        int16_t accY = (accY_HB << 8) | accY_LB;
        int16_t accZ = (accZ_HB << 8) | accZ_LB;
        Serial.print("F1: ");
        Serial.print(force_1);
        Serial.print(" F2: ");
        Serial.print(force_2);
        Serial.print(" F3: ");
        Serial.print(force_3);
        Serial.print(" F4: ");
        Serial.print(force_4);
        Serial.print(" AcX: ");
        Serial.print(accX);
        Serial.print(" AcY: ");
        Serial.print(accY);
        Serial.print(" AcZ: ");
        Serial.print(accZ);
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
