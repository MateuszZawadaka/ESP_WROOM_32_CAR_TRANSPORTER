#include "Receiver.h"

ESPNowReceiver* ESPNowReceiver::instance = nullptr;

ESPNowReceiver::ESPNowReceiver() {
    memset(&currentData, 0, sizeof(currentData));
    instance = this;
}

void ESPNowReceiver::init() {
    Serial.begin(115200);
    
    for (int i = 0; i < RELAYS_COUNT; i++) {
        pinMode(relaysPins[i], OUTPUT);
        digitalWrite(relaysPins[i], LOW);
    }

    WiFi.mode(WIFI_STA);
    esp_wifi_start();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);

    // Add master as peer
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, masterMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    sendHello();
}

void ESPNowReceiver::update() {
    // Additional logic if needed
}

void ESPNowReceiver::sendHello() {
    HelloMessage hello;
    strcpy(hello.msg, "hello");
    esp_now_send(masterMac, (uint8_t*)&hello, sizeof(hello));
}

// Static method
void ESPNowReceiver::OnDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
    if (!instance) return;
    
    memcpy(&instance->currentData, data, sizeof(instance->currentData));
    Serial.println("Data received from master:");
    
    for (int i = 0; i < RELAYS_COUNT; i++) {
        digitalWrite(instance->relaysPins[i], !instance->currentData.relaysActions[i]);
    }
}