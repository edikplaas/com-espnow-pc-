
#define DE_RE_PIN PB3

#define ADDRESS 0x35  // Fixed address
uint8_t frameCounter = 0;
void setup() {
  Serial.begin(2000000);
  delay(100);
}

void loop() {
  read_send_data();
}

void read_send_data() {
  uint8_t frame[4];
  frame[0] = ADDRESS;
  frame[1] = frameCounter++;
  uint8_t data[2];
  uint8_t data_1[2], data_2[2], data_3[2], data_4[2];

  int bytesRead_1 = I2CRead_new(0x28, data_1, data_2, data_3, data_4, sizeof(data_1));

  int bridge_data_1 = (data_1[0] << 8) | data_1[1];  // decaler msb et combiner avc lsb
  int force_1 = (bridge_data_1 & 0x3FFF); //masquer pr garder que les bits de 0-13



  data[0] = (force_1 >> 8) & 0xFF;
  data[1] = force_1 & 0xFF;
  
  for (int i = 0; i < 2; i++) {
    frame[2 + i] = data[i];
  }
  //
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1
  Serial.write(frame, 4);
  Serial.flush();
  delay(1);
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
  //
}
