#include <esp_now.h>
#include <WiFi.h>

// Définition du buffer et des variables associées
#define BUFFER_SIZE 1000 // Taille arbitraire et ajustable
uint8_t dataBuffer[BUFFER_SIZE];
size_t bufferIndex = 0;


void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  // Ajout des données reçues au buffer
  if (bufferIndex + len <= BUFFER_SIZE) {
    memcpy(&dataBuffer[bufferIndex], incomingData, len);
    bufferIndex += len;
  } else {
    // Gérer le débordement de buffer ici (par exemple, vider le buffer ou signaler une erreur)
    bufferIndex = 0; // Réinitialiser le buffer en cas de débordement
  }
}

void setup()
{
  Serial.begin(2000000);
  WiFi.mode(WIFI_STA);
  pinMode(D1,INPUT);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop()
{
  if (bufferIndex > 0) {
    // Envoyer tout le contenu du buffer via Serial
    Serial.write(digitalRead(D1));
    Serial.write(dataBuffer, bufferIndex);
    bufferIndex = 0; // Réinitialiser le buffer après traitement
  }

  
}
