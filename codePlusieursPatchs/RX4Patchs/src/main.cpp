#include <esp_now.h>
#include <WiFi.h>
// Code de réception pour l'ESP32 côté PC
// Il attend des données qui arrivent en ESP NOW puis les envoie directement au PC via le port série
// En parallèle, il lit également un bit signal envoyé depuis un programme python qui permet de mettre un top départ pour la synchro avec les plateformes de force
// Une sortie physique tout ou rien copie également ce bit signal

// Définition du buffer et des variables associées
#define BUFFER_SIZE 2960 // Taille arbitraire et ajustable
const int PIN = 2;
uint8_t dataBuffer[BUFFER_SIZE];
uint8_t broadcastAddressLeft[] = {0x8C, 0xBF, 0xEA, 0xCC, 0x9C, 0x04}; // Adresse MAC du récepteur
uint8_t broadcastAddressRight[] = {0x8C, 0xBF, 0xEA, 0xCC, 0xA8, 0xDC}; // Adresse MAC du récepteur

size_t bufferIndexGauche = 0;
size_t bufferIndexDroit = 0;
size_t bufferIndex = 0;
String receivedSignal = "0";

esp_now_peer_info_t peerInfoLeft;
esp_now_peer_info_t peerInfoRight;
bool topSynchro = false;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // S'appelle à la bonne réception de la trame envoyée
}

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
  // Serial.begin(2000000); // Pour l'ESP32C3/C6
  Serial.begin(921600); // Pour l'ESP WROOM 32
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfoLeft.peer_addr, broadcastAddressLeft, 6);
  memcpy(peerInfoRight.peer_addr, broadcastAddressRight, 6);
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  peerInfoLeft.channel = 0;
  peerInfoLeft.encrypt = false;
  peerInfoRight.channel = 0;
  peerInfoRight.encrypt = false;
  if (esp_now_add_peer(&peerInfoLeft) != ESP_OK)
  {
    return;
  }
  if (esp_now_add_peer(&peerInfoRight) != ESP_OK)
  {
    return;
  }
  delay(5000);
  uint8_t data[1];
  data[0]=100;
  esp_now_send(broadcastAddressLeft, (uint8_t *)&data, 1);
  esp_now_send(broadcastAddressRight, (uint8_t *)&data, 1);

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
    if(receivedSignal.toInt()==1 && !topSynchro){
      digitalWrite(PIN, HIGH);
      topSynchro=true;
    }
    if(receivedSignal.toInt()==0 && topSynchro){
      digitalWrite(PIN, LOW);
      topSynchro=false;
    }
    if(!topSynchro){
      Serial.write(0);
    }
    else{
      Serial.write(1);
    }
    Serial.write(dataBuffer, bufferIndex); // Ecriture de la trame
    bufferIndex = 0; // Réinitialiser le buffer après traitement
  }
    
}