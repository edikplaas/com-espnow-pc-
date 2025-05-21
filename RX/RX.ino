#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    uint8_t id; // Identifiant, false pour une semelle et true pour l'autre
    uint16_t bloc1; // 2 octets
    uint16_t bloc2; // 2 octets
    uint16_t bloc3; // 2 octets
    uint16_t bloc4; // 2 octets
    uint16_t bloc5; // 2 octets
    uint16_t bloc6; // 2 octets
    uint16_t bloc7; // 2 octets
    uint16_t bloc8; // 2 octets
    uint16_t bloc9; // 2 octets
    uint16_t bloc10; // 2 octets
    uint16_t bloc11; // 2 octets
    uint16_t bloc12; // 2 octets
    uint16_t bloc13; // 2 octets
    uint16_t bloc14; // 2 octets
    uint16_t bloc15; // 2 octets
    uint16_t bloc16; // 2 octets
    uint16_t bloc17; // 2 octets
    uint16_t bloc18; // 2 octets
    uint16_t bloc19; // 2 octets
    uint16_t bloc20; // 2 octets
    uint16_t bloc21; // 2 octets
    uint16_t bloc22; // 2 octets
    uint16_t bloc23; // 2 octets
    uint16_t bloc24; // 2 octets
    uint16_t bloc25; // 2 octets
    uint16_t bloc26; // 2 octets
    uint16_t bloc27; // 2 octets
    uint16_t bloc28; // 2 octets
    uint16_t bloc29; // 2 octets
    uint16_t bloc30; // 2 octets
    uint16_t bloc31; // 2 octets
    uint16_t bloc32; // 2 octets
    uint16_t bloc33; // 2 octets
    uint16_t bloc34; // 2 octets
    uint16_t bloc35; // 2 octets
    uint16_t bloc36; // 2 octets
    uint16_t bloc37; // 2 octets
    uint16_t bloc38; // 2 octets
    uint16_t bloc39; // 2 octets
    uint16_t bloc40; // 2 octets
    uint16_t bloc41; // 2 octets
    uint16_t bloc42; // 2 octets
    uint16_t bloc43; // 2 octets
    uint16_t bloc44; // 2 octets
    uint16_t bloc45; // 2 octets
    uint16_t bloc46; // 2 octets
    uint16_t bloc47; // 2 octets
    uint16_t bloc48; // 2 octets
    uint16_t bloc49; // 2 octets
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
    Serial.write((uint8_t *)&buffer[lastProcessedIndex], sizeof(struct_message));
    lastProcessedIndex = (lastProcessedIndex + 1) % 10;
  }
}
