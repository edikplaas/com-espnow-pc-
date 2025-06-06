#include <Arduino.h>
#define DE_RE_PIN 1 // GPIO1 pour contrôler DE/RE du transceiver RS485


void setup()
{
  Serial.begin(2000000); // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT); // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW);
}

void loop()

{
  static uint16_t t1;
  static uint16_t t2;
  static uint16_t t3;
  static uint16_t t4;
  static uint8_t patch=1;
  /* // Pour lire le nombre de trames reçues de chaque patch
   Serial.print(t1);
      Serial.print(" ");
      Serial.print(t2);
      Serial.print(" ");
      Serial.print(t3);
      Serial.print(" ");
      Serial.println(t4);
      */
  if (Serial0.available())
  {
    byte headers[2];
    Serial0.readBytes(headers, 2);
    //if ((headers[0] == 1 + (patch - 1) * 4 && headers[1] == 2 + (patch - 1) * 4)){ // Ce IF alternatif impose d'avoir le même nombre de trames de chaque patch, au prix de pertes de données
    if ((headers[0] == 1 && headers[1] == 2) || (headers[0] == 5 && headers[1] == 6) || (headers[0] == 9 && headers[1] == 10) || (headers[0] == 13 && headers[1] == 14)){
      
      byte data[3];
      Serial0.readBytes(data, 3);

      byte counter = data[0];
      byte force_1_HB = data[1];
      byte force_1_LB = data[2];
      int force_1 = (force_1_HB << 8) | force_1_LB;
      switch (headers[0])
      {
      case 1:
        t1++;
        break;
      case 5:
        t2++;
        break;
      case 9:
        t3++;
        break;
      case 13:
        t4++;
      }
      //byte frame[9]={0x00,(t1 >> 8) & 0xFF,t1 & 0xFF,(t2 >> 8) & 0xFF,t2 & 0xFF,(t3 >> 8) & 0xFF,t3 & 0xFF,(t4 >> 8) & 0xFF,t4 & 0xFF};
      //Serial.write(frame,9);
    
      Serial.print("Patch: ");
      switch(headers[0]){
        case 1:
          Serial.print("1");
          break;
        case 5:
          Serial.print("2");
          break;
        case 9:
          Serial.print("3");
          break;
        case(13):
          Serial.print("4");

      }

      Serial.print(" Counter: ");
      Serial.print(counter);
     Serial.print(" Force 1: ");
      Serial.println(force_1);

   /*
      byte frame[7];
      frame[0]=headers[0];
      frame[1]=headers[1];
      for(int i =0;i<5;i++){
        frame[i+2]=data[i];
      }
       Serial.write(frame,7);
       */
      //patch++;
      //if(patch>4){
      //  patch=1;}
    }
  }
}
