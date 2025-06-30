
#define DE_RE_PIN PB3

#define ADDRESS_1 0x35  // Fixed address
#define ADDRESS_2 0x40  // Fixed address

uint8_t frameCounter = 0;
void setup() {
  Serial.begin(2000000);
  //pinMode(DE_RE_PIN, OUTPUT);
  delay(100);
}

void loop() {
  read_send_data();
}

void read_send_data() {
  uint8_t frame[11];
  frame[0] = ADDRESS_1;
  frame[1] = ADDRESS_2;
  frame[2] = frameCounter++;
  uint8_t data[8];
  uint8_t data_1[2], data_2[2], data_3[2], data_4[2];

  int bytesRead_1 = I2CRead_new(0x28, data_1, data_3, data_2, data_4, sizeof(data_1));

  int bridge_data_1 = (data_1[0] << 8) | data_1[1];  // decaler msb et combiner avc lsb
  int force_1 = (bridge_data_1 & 0x3FFF); //masquer pr garder que les bits de 0-13

  int bridge_data_2 = (data_2[0] << 8) | data_2[1];  // decaler msb et combiner avc lsb
  int force_2 = (bridge_data_2 & 0x3FFF); //masquer pr garder que les bits de 0-13

  int bridge_data_3 = (data_3[0] << 8) | data_3[1];  // decaler msb et combiner avc lsb
  int force_3 = (bridge_data_3 & 0x3FFF); //masquer pr garder que les bits de 0-13

  int bridge_data_4 = (data_4[0] << 8) | data_4[1];  // decaler msb et combiner avc lsb
  int force_4 = (bridge_data_4 & 0x3FFF); //masquer pr garder que les bits de 0-13


  data[0] = (force_1 >> 8) & 0xFF;
  data[1] = force_1 & 0xFF;
  data[2] = (force_2 >> 8) & 0xFF;
  data[3] = force_2 & 0xFF;
  data[4] = (force_3 >> 8) & 0xFF;
  data[5] = force_3 & 0xFF;
  data[6] = (force_4 >> 8) & 0xFF;
  data[7] = force_4 & 0xFF;

  for (int i = 0; i < 8; i++) {
    frame[3 + i] = data[i];
  }
  //
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1
  Serial.write(frame, 11);
  Serial.flush();
  delay(1);
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
  //
}
