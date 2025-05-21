#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int id;
  int x;
  int y;
} struct_message;

struct_message myData;
struct_message buffer[10]; // Buffer pour stocker les données reçues
int bufferIndex = 0; // Index pour le buffer

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  // Copier les données dans le buffer
  memcpy(&buffer[bufferIndex], incomingData, sizeof(struct_message));
  bufferIndex = (bufferIndex + 1) % 10; // Incrémenter l'index de manière circulaire
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
  static int lastProcessedIndex = 0;

  // Traiter les données dans le buffer
  if (lastProcessedIndex != bufferIndex) {
    Serial.println(String(buffer[lastProcessedIndex].id) + " " +
                   String(buffer[lastProcessedIndex].x) + " " +
                   String(buffer[lastProcessedIndex].y));
    lastProcessedIndex = (lastProcessedIndex + 1) % 10;
  }
}
