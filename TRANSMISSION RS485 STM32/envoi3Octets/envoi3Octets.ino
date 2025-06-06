#define DE_RE_PIN PB3
uint8_t frame[3];

void setup() {
  Serial.begin(2000000); 
  pinMode(DE_RE_PIN, OUTPUT); 
    digitalWrite(DE_RE_PIN, HIGH);  // Mode réception initial

  delay(100);
}

void loop(){
  read_send_data();
}

void read_send_data(){
   
//    
     LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_3); //DE : high, RE : low.
     frame[0] = 0x35; // Identifiant (header)
     frame[1] = 5;    // Donnée 1
     frame[2] = 6;    // Donnée 2
     Serial.write(frame,3);
     Serial.flush();
     delay(1);
     LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_3);
     
//    
}
