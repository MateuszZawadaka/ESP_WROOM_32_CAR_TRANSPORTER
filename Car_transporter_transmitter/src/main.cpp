#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

typedef struct struct_message {
  bool relaysActions[16];
} struct_message;

typedef struct hello_message {
  char msg[6];
} hello_message;

struct_message myData;
const int switchesPin[] = {32, 33, 25, 26, 27, 14, 12, 23, 22, 21, 19, 18, 5, 17};
int statusLed = 16;

#define MAX_PEERS 20
uint8_t knownPeers[MAX_PEERS][6];
int peerCount = 0;

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

bool isPeerKnown(const uint8_t *mac) {
  for (int i = 0; i < peerCount; i++) {
    if (memcmp(mac, knownPeers[i], 6) == 0) return true;
  }
  return false;
}

void addPeer(const uint8_t *mac) {
  if (peerCount >= MAX_PEERS || isPeerKnown(mac)) return;

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) == ESP_OK) {
    memcpy(knownPeers[peerCount], mac, 6);
    peerCount++;
    Serial.println("Dodano nowego peer-a");
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  hello_message hello;
  memcpy(&hello, incomingData, sizeof(hello));

  if (strcmp(hello.msg, "hello") == 0) {
    Serial.print("Nowy odbiornik: ");
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", mac[i]);
      if (i < 5) Serial.print(":");
    }
    Serial.println();
    addPeer(mac);
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 14; i++) {
    pinMode(switchesPin[i], INPUT_PULLUP);
  }
  pinMode(statusLed, OUTPUT);

  WiFi.mode(WIFI_STA);
  readMacAddress();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  for (int i = 0; i < 14; i++) {
    myData.relaysActions[i] = digitalRead(switchesPin[i]);
  }

  for (int i = 0; i < peerCount; i++) {
    esp_now_send(knownPeers[i], (uint8_t*)&myData, sizeof(myData));
  }

  delay(100);
}
