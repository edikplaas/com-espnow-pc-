#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define DE_RE_PIN 1 // GPIO1 pour contrôler DE/RE du transceiver RS485

uint8_t broadcastAddress[] = {0x8C, 0xBF, 0xEA, 0xCC, 0xA8, 0xDC}; // Adresse MAC du récepteur
unsigned long lastMicros = 0;
unsigned long lastMillisCharge = 0;
const unsigned long interval = 3333;        // en µs
const unsigned long intervalCharge = 10000; // en ms
bool modeRecharge = false;

typedef struct struct_message
{
  uint8_t bytes[26];
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // Callback pour la confirmation d'envoi
}

void setup()
{
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

  Serial0.begin(2000000); // RS485
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW); // Mode réception initial

  // Initialiser les données fixes pour le test
}

void loop()
{
  unsigned long newTime = micros();
  unsigned long newTimeCharge = millis();

  if (Serial0.available() >= 26 && newTime-lastMicros>=interval && !modeRecharge)
  lastMicros=newTime;
  {
    uint8_t headers[2];
    Serial0.readBytes(headers, 2);
    if (headers[0] == 0x35 && headers[1] == 0x40)
    {
      myData.bytes[0] = 0x35;
      myData.bytes[1] = 0x40;
      lastMicros = newTime;
      uint8_t rawData[24];
      Serial0.readBytes(rawData, 24);
      for (int i = 0; i < 24; i++)
      {
        myData.bytes[i + 2] = rawData[i];
      }
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    }
  }

  if (newTimeCharge - lastMillisCharge >= intervalCharge)
  {
    lastMillisCharge = newTimeCharge;
    uint32_t VCC_value = 0;
    for (int i = 0; i < 16; i++)
    {
      VCC_value += analogReadMilliVolts(A0); // Lire et accumuler la tension ADC
    }
    float VCC_valueF = 2 * VCC_value / 16 / 1000.0; // Ajuster pour un diviseur 1:2 et convertir en volts

    if (VCC_valueF > 2.5)
    {
      modeRecharge = true;
    }
    else
    {
      modeRecharge = false;
    }
  }
}
