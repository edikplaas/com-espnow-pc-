#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message
{
  uint8_t bytes[26];
} struct_message;

struct_message myData;
struct_message buffer[10]; // Buffer pour stocker les données reçues
int bufferIndex = 0; // Index pour le buffer


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  memcpy(&buffer[bufferIndex], incomingData, sizeof(struct_message));
  bufferIndex = (bufferIndex + 1) % 10; // Incrémenter l'index de manière circulaire

}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop()
{
  static int lastProcessedIndex = 0;

  // Traiter les données dans le buffer
  if (lastProcessedIndex != bufferIndex) {
    Serial.write((uint8_t *)&buffer[lastProcessedIndex], sizeof(struct_message));
    lastProcessedIndex = (lastProcessedIndex + 1) % 10;
  }
}
