
#define DE_RE_PIN PB3

uint8_t patch = 4; // Patch 1 = Talon, Patch 4 = Orteils
uint8_t ADDRESS_DEBUT1=(patch-1)*4+1;
uint8_t ADDRESS_DEBUT2=(patch-1)*4+2;
uint8_t frameCounter = 0;
void setup() {
  Serial.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);
  if (patch != 1) { // Pour les patchs autres que le premier
    byte headers[2] = {0, 0};
    while (headers[0] != ADDRESS_DEBUT1-4 || headers[1] != ADDRESS_DEBUT2-4) { 
      // Le patch actuel détecte l'envoi du patch précédent avant d'entamer le loop 
      if (Serial.available()) {
        headers[0] = Serial.read();
        headers[1] = Serial.read();
      }
    }
  }
}
void loop() {
  static int oldTime = 0;
  int newTime = micros();
  if (newTime - oldTime >= 2000) { //2000 µs correspond à 500 Hz
    oldTime = newTime;
    send_data();
  }
}
void send_data() {
  uint8_t frame[5];
  frame[0] = ADDRESS_DEBUT1;
  frame[1] = ADDRESS_DEBUT2;
  frame[2] = frameCounter++;

  uint8_t data_1[2], data_2[2], data_3[2], data_4[2];
  int bytesRead_1 = I2CRead_new(0x28, data_1, data_2, data_3, data_4, sizeof(data_1));

  int bridge_data_1 = (data_1[0] << 8) | data_1[1];
  int force_1 = (bridge_data_1 & 0x3FFF);
  frame[3] = (force_1 >> 8) & 0xFF;
  frame[4] = force_1 & 0xFF;

  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1
  Serial.write(frame, 5);
  Serial.flush();
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
}
