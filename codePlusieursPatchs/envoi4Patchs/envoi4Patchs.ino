#include <SPI.h>
#define DE_RE_PIN PB3

#define SCL_1_PIN LL_GPIO_PIN_8 //PA8 //d9
#define SDA_1_PIN LL_GPIO_PIN_9 //PA9 //d1

#define SDA_2_PIN LL_GPIO_PIN_10 //PA10 //d0
#define SDA_3_PIN LL_GPIO_PIN_1 //PA1 //a1
#define SDA_4_PIN LL_GPIO_PIN_0 //PA0 //a0

#define MOSI_PIN PA7 // MOSI Pin A6
#define MISO_PIN PA6 // MISO Pin A5
#define SCLK_PIN PA5 // Clock Pin A4 

#define CS_acc PB1   // Chip Select or Slave Select Pin D6
#define CS_baro PA4   // Chip Select or Slave Select Pin A3
#define CS_mag PA3

// A CHANGER AUX BESOINS
uint8_t patch = 1; // Patch 1 => Talon, Patch 4 => Orteils
uint8_t piedDroit = 1: // piedDroit = 1 => Semelle du pied droit, piedDroit = 0 => Semelle du pied gauche
//

// A NE PAS CHANGER
uint8_t ADDRESS_DEBUT1 = 1 + (8 * piedDroit) + 2 * (patch - 1); // Identifiant 1 dans l'entête, directement calculé à partir du numéro du patch et de la semelle utilisée
uint8_t ADDRESS_DEBUT2 = 2 + (8 * piedDroit) + 2 * (patch - 1); // Identifiant 2 dans l'entête, directement calculé à partir du numéro du patch et de la semelle utilisée
uint8_t frameCounter = 0; // Compteur interne à la trame
//

void setup() {
  Serial.begin(2000000);
  SPI.setSCLK(SCLK_PIN);
  SPI.setMOSI(MOSI_PIN);
  SPI.setMISO(MISO_PIN);
  SPI.begin();
  pinMode(CS_acc, OUTPUT);
  digitalWrite(CS_acc, HIGH);
  pinMode(CS_baro, OUTPUT);
  digitalWrite(CS_baro, HIGH);
  pinMode(CS_mag, OUTPUT);
  digitalWrite(CS_mag, HIGH);
  //config imu
  writeRegister16(0x20, 0x40, 0x27); //acc
  writeRegister16(0x21, 0x40, 0x4B); //gyr
  //config baro
  writeRegister8(0x1B, 0x33); //(Normal mode, pressure enabled, temperature enabled), reg = pwr_ctrl
  writeRegister8(0x1C, 0x00); //resolution *1, reg = OSR
  initializeMMC5983MA();
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW); // Mode réception activé
}
void loop() {
  byte octet = 0;
  while (octet != 100) { // Attente de la détection du top départ envoyé par le module de com
    if (Serial.available()) { // Si des données disponibles
      octet = Serial.read(); // On lit l'octet
    }
  }
  delayMicroseconds(300 + 100 * (patch)); // Délai imposé pour respecter la commutation du mode transmission au mode réception, et également l'ordre d'envoi des données selon le numéro de patch (ordre 1 2 3 4)
  read_send_data();
}
void read_send_data() {
  uint8_t frame[36];
  frame[0] = ADDRESS_DEBUT1;
  frame[1] = ADDRESS_DEBUT2;
  frame[2] = frameCounter++;

  uint8_t data_1[2], data_2[2], data_3[2], data_4[2];
  uint8_t data_press[3];
  uint8_t data_temp[3];
  int bytesRead_1 = I2CRead_new(0x28, data_1, data_2, data_3, data_4, sizeof(data_1));


  int bridge_data_1 = (data_1[0] << 8) | data_1[1];
  int force_1 = (bridge_data_1 & 0x3FFF);

  int bridge_data_2 = (data_2[0] << 8) | data_2[1];  // decaler msb et combiner avc lsb
  int force_2 = (bridge_data_2 & 0x3FFF); //masquer pr garder que les bits de 0-13

  int bridge_data_3 = (data_3[0] << 8) | data_3[1];  // decaler msb et combiner avc lsb
  int force_3 = (bridge_data_3 & 0x3FFF); //masquer pr garder que les bits de 0-13

  int bridge_data_4 = (data_4[0] << 8) | data_4[1];  // decaler msb et combiner avc lsb
  int force_4 = (bridge_data_4 & 0x3FFF); //masquer pr garder que les bits de 0-13

  uint16_t rawAccX = read16bitRegister(0x03); // Read X-axis accelerometer
  uint16_t rawAccY = read16bitRegister(0x04); // Read Y-axis accelerometer
  uint16_t rawAccZ = read16bitRegister(0x05); // Read Z-axis accelerometer

  uint16_t rawGyrX = read16bitRegister(0x06); // Read X-axis gyroscope
  uint16_t rawGyrY = read16bitRegister(0x07); // Read Y-axis gyroscope
  uint16_t rawGyrZ = read16bitRegister(0x08); // Read Z-axis gyroscope

  readMultipleRegisters(0x04, data_press, 3); //la data pressure commence a partir de ce registre
  uint32_t pressure = ((uint32_t)data_press[2] << 16) | ((uint32_t)data_press[1] << 8) | data_press[0];
  readMultipleRegisters(0x07, data_temp, 3);
  uint32_t temp = ((uint32_t)data_temp[2] << 16) | ((uint32_t)data_temp[1] << 8) | data_temp[0]; //raw temp

  uint8_t x0 = readRegister(0x00);
  uint8_t x1 = readRegister(0x01);
  uint8_t y0 = readRegister(0x02);
  uint8_t y1 = readRegister(0x03);
  uint8_t z0 = readRegister(0x04);
  uint8_t z1 = readRegister(0x05);
  uint8_t xyz_ext = readRegister(0x06);
  if (piedDroit == 1) {
    switch (patch) {
      case 1:
        frame[3] = (force_3 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_3 & 0xFF;
        frame[5] = (force_1 >> 8) & 0xFF;
        frame[6] = force_1 & 0xFF;
        frame[7] = (force_4 >> 8) & 0xFF;
        frame[8] = force_4 & 0xFF;
        frame[9] = (force_2 >> 8) & 0xFF;
        frame[10] = force_2 & 0xFF;
        break;
      case 2 :
        frame[3] = (force_3 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_3 & 0xFF;
        frame[5] = (force_2 >> 8) & 0xFF;
        frame[6] = force_2 & 0xFF;
        frame[7] = (force_4 >> 8) & 0xFF;
        frame[8] = force_4 & 0xFF;
        frame[9] = (force_1 >> 8) & 0xFF;
        frame[10] = force_1 & 0xFF;
        break;
      case 3 :
        frame[3] = (force_3 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_3 & 0xFF;
        frame[5] = (force_1 >> 8) & 0xFF;
        frame[6] = force_1 & 0xFF;
        frame[7] = (force_4 >> 8) & 0xFF;
        frame[8] = force_4 & 0xFF;
        frame[9] = (force_2 >> 8) & 0xFF;
        frame[10] = force_2 & 0xFF;
        break;
      case 4:
        frame[3] = (force_3 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_3 & 0xFF;
        frame[5] = (force_1 >> 8) & 0xFF;
        frame[6] = force_1 & 0xFF;
        frame[7] = (force_4 >> 8) & 0xFF;
        frame[8] = force_4 & 0xFF;
        frame[9] = (force_2 >> 8) & 0xFF;
        frame[10] = force_2 & 0xFF;
    }
  }
  else if (piedDroit == 0) {
    switch (patch) {
      case 1:
        frame[3] = (force_1 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_1 & 0xFF;
        frame[5] = (force_3 >> 8) & 0xFF;
        frame[6] = force_3 & 0xFF;
        frame[7] = (force_2 >> 8) & 0xFF;
        frame[8] = force_2 & 0xFF;
        frame[9] = (force_4 >> 8) & 0xFF;
        frame[10] = force_4 & 0xFF;
        break;
      case 2 :
        frame[3] = (force_2 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_2 & 0xFF;
        frame[5] = (force_3 >> 8) & 0xFF;
        frame[6] = force_3 & 0xFF;
        frame[7] = (force_1 >> 8) & 0xFF;
        frame[8] = force_1 & 0xFF;
        frame[9] = (force_4 >> 8) & 0xFF;
        frame[10] = force_4 & 0xFF;
        break;
      case 3 :
        frame[3] = (force_1 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_1 & 0xFF;
        frame[5] = (force_3 >> 8) & 0xFF;
        frame[6] = force_3 & 0xFF;
        frame[7] = (force_2 >> 8) & 0xFF;
        frame[8] = force_2 & 0xFF;
        frame[9] = (force_4 >> 8) & 0xFF;
        frame[10] = force_4 & 0xFF;
        break;
      case 4:
        frame[3] = (force_3 >> 8) & 0xFF; // Décomposition des octets
        frame[4] = force_3 & 0xFF;
        frame[5] = (force_4 >> 8) & 0xFF;
        frame[6] = force_4 & 0xFF;
        frame[7] = (force_2 >> 8) & 0xFF;
        frame[8] = force_2 & 0xFF;
        frame[9] = (force_1 >> 8) & 0xFF;
        frame[10] = force_1 & 0xFF;
    }
  }

  frame[11]  = (rawAccX >> 8) & 0xFF;
  frame[12]  = rawAccX & 0xFF;
  frame[13] = (rawAccY >> 8) & 0xFF;
  frame[14] = rawAccY & 0xFF;
  frame[15] = (rawAccZ >> 8) & 0xFF;
  frame[16] = rawAccZ & 0xFF;
  frame[17] = (rawGyrX >> 8) & 0xFF;
  frame[18] = rawGyrX & 0xFF;
  frame[19] = (rawGyrY >> 8) & 0xFF;
  frame[20] = rawGyrY & 0xFF;
  frame[21] = (rawGyrZ >> 8) & 0xFF;
  frame[22] = rawGyrZ & 0xFF;

  frame[23] = (pressure >> 16) & 0xFF;
  frame[24] = (pressure >> 8) & 0xFF;
  frame[25] = pressure & 0xFF;
  frame[26] = (temp >> 16) & 0xFF;
  frame[27] = (temp >> 8) & 0xFF;
  frame[28] = temp & 0xFF;

  frame[29] = x0;
  frame[30] = x1;
  frame[31] = y0;
  frame[32] = y1;
  frame[33] = z0;
  frame[34] = z1;
  frame[35] = xyz_ext;
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3); //DE : high, RE : low.1
  Serial.write(frame, 36);
  Serial.flush();
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
}


void readMultipleRegisters(uint8_t startReg, uint8_t *data, uint8_t count) {

  digitalWrite(CS_baro, LOW);  // Select the BMP384

  SPI.transfer(startReg | 0x80);   // Send start register address with read bit (bit 7 set to 1)

  SPI.transfer(0x00);         // Send dummy byte
  for (uint8_t i = 0; i < count; i++) {
    data[i] = SPI.transfer(0x00);  // Read the data bytes
  }
  digitalWrite(CS_baro, HIGH);  // Deselect the BMP384
}

//read 2 bytes
uint16_t read16bitRegister(uint8_t reg) {

  digitalWrite(CS_acc, LOW);  // Select the BMI323
  SPI.transfer(reg | 0x80);   // Send register address with read command (bit 7 set)

  SPI.transfer(0x00); //dummy read
  uint8_t lowByte = SPI.transfer(0x00);  //P22, accelerometer, P23, gyroscope
  uint8_t highByte = SPI.transfer(0x00);

  uint16_t value = (highByte << 8) | lowByte;  // Read 16 bits (2 bytes)
  digitalWrite(CS_acc, HIGH);  // Deselect the BMI323
  return value;
}

//read 1 byte
uint16_t read8bitRegister(uint8_t reg) {

  SPI.transfer(reg | 0x80);   // Send register address with read command (bit 7 set)

  SPI.transfer(0x00); //dummy read

  uint16_t value = SPI.transfer(0x00);
  return value;
}

//write 2 bytes
void writeRegister16(byte address, uint16_t value1, uint16_t value2) {
  digitalWrite(CS_acc, LOW);  // Select the sensor

  uint16_t addr = address & 0x7F;

  // Send the register address
  SPI.transfer(addr);

  uint16_t value = (value2 << 8) | value1; // decaler msb et combiner avc lsb
  SPI.transfer16(value);

  digitalWrite(CS_acc, HIGH);  // Deselect the sensor

}


//write 1 byte
void writeRegister8(byte address, uint16_t value) {
  digitalWrite(CS_baro, LOW);  // Select the sensor

  uint16_t ctrl = address & 0x7F;

  // Send the register address
  SPI.transfer(ctrl);

  SPI.transfer(value);

  digitalWrite(CS_baro, HIGH);  // Deselect the sensor

}


void initializeMMC5983MA() {
  writeRegister(0x0A, 0b00000011);  // Bande passante : 0.5ms (800Hz) → BW1=1, BW0=1
  writeRegister(0x09, 0b00100000);  // Auto-Set activé
  writeRegister(0x0B, 0b10011111);   //  Mode mesure continue à 1000 Hz (CM_freq=111), Set automatique tous les 75 échantillons (Prd_set=001),Bit 7 = En_prd_set, bit 3 = Cmm_en

  delay(10);
}

void writeRegister(uint8_t reg, uint8_t value) {
  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_mag, LOW);
  SPI.transfer((reg & 0x3F) | 0x00);  // Write: MSB = 0
  SPI.transfer(value);
  digitalWrite(CS_mag, HIGH);
  //SPI.endTransaction();
}

uint8_t readRegister(uint8_t reg) {
  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_mag, LOW);
  SPI.transfer((reg & 0x3F) | 0x80);  // Read: MSB = 1
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(CS_mag, HIGH);
  //SPI.endTransaction();
  return value;
}
