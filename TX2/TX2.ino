#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x8C, 0xBF, 0xEA, 0xCC, 0x9C, 0x04}; // Adresse MAC du récepteur
unsigned long lastMicros = 0;
const unsigned long interval = 2000; // en µs, 2000µs correspond à 500Hz, 10000µs correspond à 100Hz

typedef struct struct_message {
    uint8_t id; // 1 octet pour l'identifiant
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
  myData.id = 2;
}
 
void loop() {
  unsigned long newTime = micros();
  static int nb = 0;
  if (newTime - lastMicros >= interval) {
    lastMicros += interval;
    myData.bloc1 = nb;
    myData.bloc2 = nb;
    myData.bloc3 = nb;
    myData.bloc4 = nb;
    myData.bloc5 = nb;
    myData.bloc6 = nb;
    myData.bloc7 = nb;
    myData.bloc8 = nb;
    myData.bloc9 = nb;
    myData.bloc10 = nb;
    myData.bloc11 = nb;
    myData.bloc12 = nb;
    myData.bloc13 = nb;
    myData.bloc14 = nb;
    myData.bloc15 = nb;
    myData.bloc16 = nb;
    myData.bloc17 = nb;
    myData.bloc18 = nb;
    myData.bloc19 = nb;
    myData.bloc20 = nb;
    myData.bloc21 = nb;
    myData.bloc22 = nb;
    myData.bloc23 = nb;
    myData.bloc24 = nb;
    myData.bloc25 = nb;
    myData.bloc26 = nb;
    myData.bloc27 = nb;
    myData.bloc28 = nb;
    myData.bloc29 = nb;
    myData.bloc30 = nb;
    myData.bloc31 = nb;
    myData.bloc32 = nb;
    myData.bloc33 = nb;
    myData.bloc34 = nb;
    myData.bloc35 = nb;
    myData.bloc36 = nb;
    myData.bloc37 = nb;
    myData.bloc38 = nb;
    myData.bloc39 = nb;
    myData.bloc40 = nb;
    myData.bloc41 = nb;
    myData.bloc42 = nb;
    myData.bloc43 = nb;
    myData.bloc44 = nb;
    myData.bloc45 = nb;
    myData.bloc46 = nb;
    myData.bloc47 = nb;
    myData.bloc48 = nb;
    myData.bloc49 = nb;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    // result permet de vérifier le bon envoi des données
    nb++;
  }
}
