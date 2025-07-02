#include <Arduino.h>
#define DE_RE_PIN 1 // GPIO1 pour contrôler DE/RE du transceiver RS485
const double TWO_POWER_65 = 36893488147419103232.0;

void setup()
{
  Serial.begin(2000000); // Moniteur série pour débogage
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);   // Configurer GPIO1 comme sortie
  digitalWrite(DE_RE_PIN, LOW); // Mode réception initial
}

void receive_data()
{
  if (Serial0.available() >= 44)
  { // Vérifie si des données sont disponibles
    byte frame[44];
    Serial0.readBytes(frame, 44);
    if (frame[0] == 1)
    {
      Serial.print(frame[0]);
      Serial.print(frame[1]);
      Serial.print(" ");
      uint32_t uncomp_temp = (frame[2] << 24) | (frame[3] << 16) | (frame[4] << 8) | (frame[5]);
      Serial.print(uncomp_temp);
      Serial.print(" ");
      uint16_t NVM_PAR_T1 = frame[6] << 8 | frame[7];
      uint16_t NVM_PAR_T2 = frame[8] << 8 | frame[9];
      uint16_t rawNVM_PAR_T3 = frame[10] << 8 | frame[11];
      int8_t NVM_PAR_T3 = rawNVM_PAR_T3 - 128;
      Serial.print(NVM_PAR_T1);
      Serial.print(" ");
      Serial.print(NVM_PAR_T2);
      Serial.print(" ");
      Serial.print(NVM_PAR_T3);
      Serial.print(" ");
      float PAR_T1 = NVM_PAR_T1 * float(1 << 8);
      float PAR_T2 = NVM_PAR_T2 / float(1L << 30);
      double PAR_T3 = NVM_PAR_T3 / double(1ULL << 48);
      float partial_data1_temp = (float)(uncomp_temp - PAR_T1);
      float partial_data2_temp = (float)(partial_data1_temp * PAR_T2);
      float t_lin = partial_data2_temp + (partial_data1_temp * partial_data1_temp) * PAR_T3;
      Serial.print(t_lin);
      Serial.print("   ");
      uint32_t uncomp_press = (frame[12] << 24) | (frame[13] << 16) | (frame[14] << 8) | (frame[15]);
      Serial.print(uncomp_press);
      Serial.print(" ");
      uint32_t rawNVM_PAR_P1 = (frame[16] << 24) | (frame[17] << 16) | (frame[18] << 8) | (frame[19]);
      uint32_t rawNVM_PAR_P2 = (frame[20] << 24) | (frame[21] << 16) | (frame[22] << 8) | (frame[23]);
      uint16_t rawNVM_PAR_P3 = (frame[24] << 8) | (frame[25]);
      uint16_t rawNVM_PAR_P4 = (frame[26] << 8) | (frame[27]);
      uint16_t NVM_PAR_P5 = (frame[28] << 8) | (frame[29]);
      uint16_t NVM_PAR_P6 = (frame[30] << 8) | (frame[31]);
      uint16_t rawNVM_PAR_P7 = (frame[32] << 8) | (frame[33]);
      uint16_t rawNVM_PAR_P8 = (frame[34] << 8) | (frame[35]);
      uint32_t rawNVM_PAR_P9 = (frame[36] << 24) | (frame[37] << 16) | (frame[38] << 8) | (frame[39]);
      uint16_t rawNVM_PAR_P10 = (frame[40] << 8) | (frame[41]);
      uint16_t rawNVM_PAR_P11 = (frame[42] << 8) | (frame[43]);

      int16_t NVM_PAR_P1 = -32768 + rawNVM_PAR_P1;
      int16_t NVM_PAR_P2 = -32768 + rawNVM_PAR_P2;
      int8_t NVM_PAR_P3 = -128 + rawNVM_PAR_P3;
      int8_t NVM_PAR_P4 = -128 + rawNVM_PAR_P4;

      int8_t NVM_PAR_P7 = -128 + rawNVM_PAR_P7;
      int8_t NVM_PAR_P8 = -128 + rawNVM_PAR_P8;
      int16_t NVM_PAR_P9 = -32768 + rawNVM_PAR_P9;
      int8_t NVM_PAR_P10 = -128 + rawNVM_PAR_P10;
      int8_t NVM_PAR_P11 = -128 + rawNVM_PAR_P11;
      float PAR_P1 = (NVM_PAR_P1 - float(1 << 14)) / float(1 << 20);
      float PAR_P2 = (NVM_PAR_P2 - float(1 << 14)) / float(1L << 29);
      double PAR_P3 = NVM_PAR_P3 / double(1ULL << 32);
      double PAR_P4 = NVM_PAR_P4 / double(1ULL << 37);
      float PAR_P5 = NVM_PAR_P5 * float(1 << 3);
      float PAR_P6 = NVM_PAR_P6 / float(1 << 6);
      float PAR_P7 = NVM_PAR_P7 / float(1 << 8);
      float PAR_P8 = NVM_PAR_P8 / float(1 << 15);
      double PAR_P9 = NVM_PAR_P9 / double(1ULL << 48);
      double PAR_P10 = NVM_PAR_P10 / double(1ULL << 48);
      double PAR_P11 = NVM_PAR_P11 / TWO_POWER_65;

      // apply compensation
      float partial_data1 = (float)(PAR_P6 * t_lin);
      float partial_data2 = (float)(PAR_P7 * t_lin * t_lin);
      float partial_data3 = (float)(PAR_P8 * t_lin * t_lin * t_lin);
      float partial_out1 = (float)(PAR_P5 + partial_data1 + partial_data2 + partial_data3);

      partial_data1 = (float)(PAR_P2 * t_lin);
      partial_data2 = (float)(PAR_P3 * t_lin * t_lin);
      partial_data3 = (float)(PAR_P4 * t_lin * t_lin * t_lin);
      float partial_out2 = (float)(uncomp_press * PAR_P1 + partial_data1 + partial_data2 + partial_data3);

      partial_data1 = (float)(uncomp_press) * (float)(uncomp_press);
      partial_data2 = PAR_P9 + PAR_P10 * t_lin;
      partial_data3 = partial_data1 * partial_data2;
      float partial_data4 = partial_data3 + (float)(uncomp_press) * (float)(uncomp_press) * (float)(uncomp_press)*PAR_P11;

      float comp_press = partial_out1 + partial_out2 + partial_data4;
      Serial.print(NVM_PAR_P1); Serial.print(" "); 
      Serial.print(NVM_PAR_P2); Serial.print(" "); 
      Serial.print(NVM_PAR_P3); Serial.print(" "); 
      Serial.print(NVM_PAR_P4); Serial.print(" "); 
      Serial.print(NVM_PAR_P5); Serial.print(" "); 
      Serial.print(NVM_PAR_P6); Serial.print(" "); 
      Serial.print(NVM_PAR_P7); Serial.print(" "); 
      Serial.print(NVM_PAR_P8); Serial.print(" "); 
      Serial.print(NVM_PAR_P9); Serial.print(" "); 
      Serial.print(NVM_PAR_P10); Serial.print(" "); 
      Serial.print(NVM_PAR_P11); Serial.print(" "); 
      Serial.print(comp_press);
      Serial.println();
    }
  }
}
void loop()
{
  receive_data();
}
