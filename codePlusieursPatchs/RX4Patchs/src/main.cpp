#include <esp_now.h>
#include <WiFi.h>
// Code de réception pour l'ESP32C3 côté PC
// Il attend des données qui arrivent en ESP NOW puis les envoient directement au PC via le port série
// En parallèle, il lit également un bit signal envoyé depuis un programme python qui permet de mettre un top départ pour la synchro

// Définition du buffer et des variables associées
#define BUFFER_SIZE 1000 // Taille arbitraire et ajustable
uint8_t dataBuffer[BUFFER_SIZE];
size_t bufferIndex = 0;
String receivedSignal = "0";
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  // Ajout des données reçues au buffer
  if (bufferIndex + len <= BUFFER_SIZE)
  {
    memcpy(&dataBuffer[bufferIndex], incomingData, len);
    bufferIndex += len;
  }
  else
  {
    // Gérer le débordement de buffer ici (par exemple, vider le buffer ou signaler une erreur)
    bufferIndex = 0; // Réinitialiser le buffer en cas de débordement
  }
}

void setup()
{
  Serial.begin(2000000);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop()
{
  if (bufferIndex > 0)
  {
    // Envoyer tout le contenu du buffer via Serial
    if (Serial.available()) // Si des données viennent du PC
    {
      receivedSignal = Serial.readStringUntil('\n'); // Lecture jusqu'à fin de ligne
    }
    Serial.write(receivedSignal.toInt()); // Ecriture du bit signal en début de trame
    Serial.write(dataBuffer, bufferIndex); // Ecriture de la trame
    bufferIndex = 0; // Réinitialiser le buffer après traitement
  }
}