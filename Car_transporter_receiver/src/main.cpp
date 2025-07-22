#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

int relaysPin[] = {32, 33, 25, 26, 27, 14, 12, 17, 5, 18, 19, 21, 22, 23};
const uint8_t masterMac[] = {0xEC, 0x64, 0xC9, 0x85, 0xF7, 0x50}; // MAC nadajnika
typedef struct struct_message {
  bool relaysActions[16];
} struct_message;

typedef struct hello_message {
  char msg[6];
} hello_message;

struct_message myData;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("Data received from master:");
  for (int i = 0; i < 14; i++) {
    digitalWrite(relaysPin[i], !myData.relaysActions[i]);
  }
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 14; i++) {
    pinMode(relaysPin[i], OUTPUT);
  }

  WiFi.mode(WIFI_STA);
  esp_wifi_start();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // Dodaj mastera jako peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, masterMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  // Wyślij hello
  hello_message hello;
  strcpy(hello.msg, "hello");
  esp_now_send(masterMac, (uint8_t*)&hello, sizeof(hello));
}

void loop() {
  if (WiFi.status() != WL_CONNECTION_LOST)
  {
      for (int i = 0; i < 14; i++)
      {
        digitalWrite(relaysPin[i], false);
        delay(100);
      }

  }
  
}
