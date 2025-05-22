#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x8C, 0xBF, 0xEA, 0xCC, 0x9C, 0x04}; // Adresse MAC du récepteur
unsigned long lastMicros = 0;
const unsigned long interval = 2000; // en µs, 2000µs correspond à 500Hz, 10000µs correspond à 100Hz

typedef struct struct_message {
  uint8_t entete1;
  uint8_t entete2;
  uint8_t bytes[30];

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

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return;
  }
  myData.entete1 = 99;
  myData.entete2 = 101;
}

void loop() {
  unsigned long newTime = micros();
  static uint8_t nb = 0;
  if (newTime - lastMicros >= interval) {
    lastMicros += interval;
    for (int i = 0; i < 30; i++) {
      myData.bytes[i] = nb;
    }
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    // result permet de vérifier le bon envoi des données
    nb++;
  }
}
