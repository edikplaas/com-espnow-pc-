#define DE_RE_PIN PB3

void setup() {
  Serial.begin(2000000); 
  delay(100);
}
void loop(){
  read_send_data();
}
void read_send_data(){
     LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_3); //DE : high, RE : low.
     Serial.write(5);
     Serial.flush();
     delay(10);
     LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_3); 
}
