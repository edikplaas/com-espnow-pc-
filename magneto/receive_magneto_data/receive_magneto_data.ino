#define DE_RE_PIN 1
uint8_t frame[9];
uint8_t frameIndex = 0;
bool receiving = false;
int16_t mx, my, mz;

void setup() {
  Serial.begin(2000000);
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);  // Receive mode
}


void receive_data() {
  while (Serial0.available() >=9) {
    if (Serial0.read() == 0xAA){
      uint8_t productID = Serial0.read();
//      uint8_t f2 = Serial0.read();
//      float tempC = -75.0 + f2 * 0.8;
      uint8_t x0 = Serial0.read();
      uint8_t x1 = Serial0.read();
      uint8_t y0 = Serial0.read();
      uint8_t y1 = Serial0.read();
      uint8_t z0 = Serial0.read();
      uint8_t z1 = Serial0.read();
      uint8_t xyz_ext = Serial0.read();

      // Assemble 18-bit raw values
      uint32_t x_raw = ((uint32_t)x0 << 10) | ((uint32_t)x1 << 2) | ((xyz_ext >> 6) & 0x03);
      uint32_t y_raw = ((uint32_t)y0 << 10) | ((uint32_t)y1 << 2) | ((xyz_ext >> 4) & 0x03);
      uint32_t z_raw = ((uint32_t)z0 << 10) | ((uint32_t)z1 << 2) | ((xyz_ext >> 2) & 0x03);
    
      // Convert to 16-bit signed (centered around 0)
      mx = (int32_t)x_raw - 131072; //raw values
      my = (int32_t)y_raw - 131072;
      mz = (int32_t)z_raw - 131072;
      
//      Serial.print("Product ID: 0x");
//      Serial.println(productID, HEX);

//      Serial.print("temp_raw:");
//      Serial.println(f2);
//      Serial.print("temperature :");
//      Serial.print(tempC);
//      Serial.println(" °C");

      float fx = mx * 0.00625f; //en microteslas sinon *0.0625f;  pour mG milligauss
      float fy = my * 0.00625f;
      float fz = mz * 0.00625f;

      Serial.print("| X = "); Serial.print(fx);
      Serial.print(" | Y = "); Serial.print(fy);
      Serial.print(" | Z = "); Serial.println(fz);

      float norm = sqrt(fx*fx + fy*fy + fz*fz);
      Serial.print("Norme = "); Serial.println(norm);

    }
    }
}

void loop() {
  receive_data();
}
