#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

/*
La communication se passe de cette manière :
L'ESP du module de com envoie un "top départ" (un octet spécial) aux 4 patchs à une fréquence précise
A chaque fois que les 4 patchs reçoivent le top départ, ils envoient leurs données en respectant l'ordre patch 1 - 2 - 3 - 4 à l'aide d'un délai appliqué avant la transmission
Chaque trame est lue, mais l'envoi sans fil en ESP NOW se fait que lorsque les 4 trames des patchs sont reçues et stockées pour éviter de saturer la com ESP NOW tout en respectant
la fréquence de 250Hz
*/

#define DE_RE_PIN 1                                                // GPIO1 pour contrôler DE/RE du transceiver RS485
uint8_t broadcastAddress[] = {0xDC, 0xDA, 0x0C, 0xA1, 0x95, 0xBC}; // Adresse MAC du récepteur
unsigned long lastMillisCharge = 0;
const unsigned long intervalleCharge = 5000;    // intervalle en ms de la vérification du mode recharge (Toutes les 5 secondes, on vérifie si l'ESP est branché au PC)
const unsigned long intervalleTopDepart = 4000; // intervalle en µs qui correspond à la période entre chaque envoi du top départ qui permet de récupèrer les 4 trames respectives des patchs
// 4000 µs => 250 Hz

bool modeRecharge = false; // true si l'ESP est branché au pc, false sinon

typedef struct struct_message
{ // Trame unique de 36 octets de data
  uint8_t bytes[36];
} struct_message;

typedef struct struct_combined_message
{                    // Grosse trame combinée de 4 trames (les 4 patchs) pour l'envoi en ESP NOW
  uint8_t bytes[144]; // 23 octets * 4 trames
} struct_combined_message;

struct_combined_message combinedData;
int trameCount = 0; // Compteur pour le nombre de trames stockées

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // S'appelle à la bonne réception de la trame envoyée
}

void gestionCharge() // Fonction pour la gestion de la détection d'alimentation PC
{ 
  unsigned long newTimeCharge = millis();
  if (newTimeCharge - lastMillisCharge >= intervalleCharge)
  {
    lastMillisCharge = newTimeCharge;
    uint32_t VCC_value = 0;
    for (int i = 0; i < 16; i++)
    {
      VCC_value += analogReadMilliVolts(A0); // Read and accumulate ADC voltage
    }
    float VCC_valueF = 2 * VCC_value / 16 / 1000.0; // Adjust for 1:2 divider and convert to volts
    if (VCC_valueF > 2.5)
    {
      modeRecharge = true; // L'ESP est branché au PC
    }
    else
    {
      modeRecharge = false; // L'ESP n'est pas branché au PC
    }
  }
}

void envoiTopDepart() // Fonction pour l'envoi du top départ aux 4 patchs
{
  static int oldTime = 0;
  int newTime = micros(); 
  if (newTime - oldTime >= intervalleTopDepart && !Serial0.available() && !modeRecharge)
  { // Envoi du top départ à 250 Hz (période 4000µs)
    oldTime = newTime;
    digitalWrite(DE_RE_PIN, HIGH); // Mode transmission
    Serial0.write(100);            // Octet spécial pour le top départ
    Serial0.flush();
    digitalWrite(DE_RE_PIN, LOW); // Mode réception
  }
}

void setup()
{
  Serial0.begin(2000000);
  pinMode(DE_RE_PIN, OUTPUT); // Configurer GPIO1 comme sortie
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    return;
  }
}

void loop()
{
  if (Serial0.available() && !modeRecharge) // Si il y a des données dispo à la lecture et que l'ESP n'est pas branché au PC
  { // On lit les données et on les stocke
    byte headers[2]; // Récupère l'entête (2 octets)
    Serial0.readBytes(headers, 2);

    int trameType = -1;
    if (headers[0] == 1 && headers[1] == 2)
      trameType = 0; // Identification du patch
    else if (headers[0] == 3 && headers[1] == 4)
      trameType = 1;
    else if (headers[0] == 5 && headers[1] == 6)
      trameType = 2;
    else if (headers[0] == 7 && headers[1] == 8)
      trameType = 3;

    if (trameType != -1)
    {
      byte data[34];
      Serial0.readBytes(data, 34); // Lecture des 24 octets de data après l'entête

      // Calculer la position de la trame dans le tableau combiné
      int startPos = trameType * 36;
      combinedData.bytes[startPos] = headers[0]; // Stockage des entêtes
      combinedData.bytes[startPos + 1] = headers[1];
      for (int i = 0; i < 34; i++)
      { // Stockage de la data
        combinedData.bytes[startPos + 2 + i] = data[i];
      }
      trameCount++;

      // Envoyer toutes les trames ensemble si toutes ont été reçues
      if (trameCount == 4)
      { // Si les 4 trames des patchs (1, 2, 3 et 4) reçues, alors envoi des 4 trames d'un coup
        esp_now_send(broadcastAddress, (uint8_t *)&combinedData, 144);
        trameCount = 0; // Réinitialiser le compteur après l'envoi
      }
    }
  }
  envoiTopDepart();
  gestionCharge(); // Détection du mode recharge
}
