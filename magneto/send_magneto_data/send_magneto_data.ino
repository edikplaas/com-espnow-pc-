#include <SPI.h>
#define DE_RE_PIN PB3

#define CS_PIN PA3  // Chip Select or Slave Select Pin A2

#define MOSI_PIN PA7 // MOSI Pin A6
#define MISO_PIN PA6 // MISO Pin A5
#define SCLK_PIN PA5 // Clock Pin A4 



void setup() {
  Serial.begin(2000000);
  delay(100);

  SPI.setSCLK(SCLK_PIN);
  SPI.setMOSI(MOSI_PIN);
  SPI.setMISO(MISO_PIN);
  
  // Initialize SPI
  SPI.begin();

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  delay(100);

  initializeMMC5983MA();  
  

  
}

void loop() {
  // Check Product ID (should return 0x30)
  uint8_t productID = readRegister(0x2F);

//  writeRegister(0x09, 0x0A);  // TM_T = 1 and STR = 1
//  delay(1); 
//  uint8_t temp_raw = readRegister(0x07);

//  writeRegister(0x09, 0x01);  // STR (Set pulse)#mesure à la demande
//  delay(1);

  uint8_t x0 = readRegister(0x00);
  uint8_t x1 = readRegister(0x01);
  uint8_t y0 = readRegister(0x02);
  uint8_t y1 = readRegister(0x03);
  uint8_t z0 = readRegister(0x04);
  uint8_t z1 = readRegister(0x05);
  uint8_t xyz_ext = readRegister(0x06);
  

 

  // Prepare frame
  uint8_t frame[9];
  frame[0] = 0xAA;  
  frame[1] = productID;   
  frame[2]=x0;
  frame[3]=x1;
  frame[4]=y0;
  frame[5]=y1;
  frame[6]=z0;
  frame[7]=z1;
  frame[8]=xyz_ext;
  


  LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_3);
  
  for (int i = 0; i < 9; i++) {
    Serial.write(frame[i]);
  }
  Serial.flush();
  LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_3);



}


void initializeMMC5983MA() {
  writeRegister(0x0A, 0b00000011);  // Bande passante : 0.5ms (800Hz) → BW1=1, BW0=1
  writeRegister(0x09, 0b00100000);  // Auto-Set activé
  writeRegister(0x0B, 0b10011111);   //  Mode mesure continue à 1000 Hz (CM_freq=111), Set automatique tous les 75 échantillons (Prd_set=001),Bit 7 = En_prd_set, bit 3 = Cmm_en
  
  delay(10);
}

void writeRegister(uint8_t reg, uint8_t value) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW);
  SPI.transfer((reg & 0x3F) | 0x00);  // Write: MSB = 0
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
}

uint8_t readRegister(uint8_t reg) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_PIN, LOW);
  SPI.transfer((reg & 0x3F) | 0x80);  // Read: MSB = 1
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  return value;
}
