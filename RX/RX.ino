#include <esp_now.h>
#include <WiFi.h>
typedef struct struct_message { // Constitution de la trame de réception
  int id; // Identifiant de trame
  int x; // Première donnée
  int y; // Deuxième donnée
}struct_message;

struct_message myData;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  memcpy(&myData, incomingData, sizeof(myData));  
  Serial.print(myData.id);
  Serial.print(" ");
  Serial.print(myData.x);
  Serial.print(" ");
  Serial.println(myData.y); // Envoi de la trame par le port série pour le programme Python
}
 
void setup() {
  Serial.begin(2000000);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
void loop() {
}
