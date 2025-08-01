#include "Transmitter.h"

ESPNowTransmitter* ESPNowTransmitter::instance = nullptr;

ESPNowTransmitter::ESPNowTransmitter() {
    memset(knownPeers, 0, sizeof(knownPeers));
    memset(&currentData, 0, sizeof(currentData));
    instance = this;
}

void ESPNowTransmitter::init() {
    Serial.begin(115200);
    
    for (int i = 0; i < SWITCHES_COUNT; i++) {
        pinMode(switchesPins[i], INPUT_PULLUP);
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

void ESPNowTransmitter::update() {
    readSwitches();
    
    for (int i = 0; i < peerCount; i++) {
        esp_now_send(knownPeers[i], (uint8_t*)&currentData, sizeof(currentData));
    }
}

void ESPNowTransmitter::readMacAddress() {
    uint8_t baseMac[6];
    if (esp_wifi_get_mac(WIFI_IF_STA, baseMac) == ESP_OK) {
        Serial.printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                     baseMac[0], baseMac[1], baseMac[2],
                     baseMac[3], baseMac[4], baseMac[5]);
    }
}

void ESPNowTransmitter::readSwitches() {
    for (int i = 0; i < SWITCHES_COUNT; i++) {
        currentData.relaysActions[i] = digitalRead(switchesPins[i]);
    }
}

bool ESPNowTransmitter::isPeerKnown(const uint8_t* mac) {
    for (int i = 0; i < peerCount; i++) {
        if (memcmp(mac, knownPeers[i], 6) == 0) return true;
    }
    return false;
}

void ESPNowTransmitter::addPeer(const uint8_t* mac) {
    if (peerCount >= MAX_PEERS || isPeerKnown(mac)) return;

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) == ESP_OK) {
        memcpy(knownPeers[peerCount], mac, 6);
        peerCount++;
        Serial.println("Added new peer");
    }
}

void ESPNowTransmitter::OnDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
    if (!instance) return;
    
    HelloMessage hello;
    memcpy(&hello, data, sizeof(hello));

    if (strcmp(hello.msg, "hello") == 0) {
        Serial.print("New receiver: ");
        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X", mac[i]);
            if (i < 5) Serial.print(":");
        }
        Serial.println();
        instance->addPeer(mac);
    }
}

void ESPNowTransmitter::OnDataSent(const uint8_t* mac, esp_now_send_status_t status) {
    if (!instance) return;
    
    Serial.print("Send status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}