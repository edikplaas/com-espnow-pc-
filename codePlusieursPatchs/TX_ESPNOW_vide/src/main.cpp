#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

/*
Code de transmission pour le module de communication du pied droit
La communication se passe de cette manière :
L'ESP du module de com envoie un "top départ" (un octet spécial) aux 4 patchs à une fréquence précise (ici 250 Hz avec une période de 4000 µs)
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

esp_now_peer_info_t peerInfo;

bool piedDroit = true; // A changer selon la semelle utilisée, true => semelle droite, false => semelle gauche

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // S'appelle à la bonne réception de la trame envoyée
}

void setup()
{
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
  if(piedDroit){
    delayMicroseconds(2000);
  }
}

void loop()
{
  static int oldTime = micros();
  int newTime = micros();
  static uint8_t cpt=0;
  static byte data[144];

  for (int i = 0; i < 4; i++)
  {
    int startPos = i * 36;

    if(piedDroit){
      data[startPos] = 9+2*i; // Stockage des entêtes
      data[startPos + 1] = 10+2*i;
    }
    else{
      data[startPos] = 1+2*i; // Stockage des entêtes
      data[startPos + 1] = 2+2*i;
    }
    for (int j = 0; j < 34; j++)
    { // Stockage de la data
      data[startPos + 2 + j] = cpt;
    }
  }
  // Envoyer toutes les trames ensemble si toutes ont été reçues
  if (newTime - oldTime >= 4000)
  { // Si les 4 trames des patchs (1, 2, 3 et 4) reçues, alors envoi des 4 trames d'un coup
    oldTime = newTime;
    esp_now_send(broadcastAddress, (uint8_t *)&data, 144);
    cpt++;
  }
}
