#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x8C, 0xBF, 0xEA, 0xCC, 0x9C, 0x04}; // Adresse MAC du récepteur
unsigned long lastMicros = 0;
const unsigned long interval = 10000; // 10 ms = 10000 µs

typedef struct struct_message { // Constitution de la trame d'envoi
    int id; // Identifiant 
    int x; // Donnée 1
    int y; // Donnée 2
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // S'appelle à la bonne réception de la trame envoyée
}
 
void setup() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    return;
  }
}
 
void loop() {
  unsigned long newTime = micros();
  static int nb = 0;
  if (now - lastMicros >= interval) {
    lastMicros += interval;
    myData.id = 2;
    myData.x = nb;
    myData.y = 500 - nb;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    // result permet de vérifier le bon envoi des données
    nb++;
    if (nb>500){
      nb=0;
    }
  }
}
