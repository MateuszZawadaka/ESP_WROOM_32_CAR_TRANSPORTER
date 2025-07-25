#pragma once
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define MAX_PEERS 20
#define SWITCHES_COUNT 14
#define RELAYS_COUNT 16

class ESPNowTransmitter {
private:
    struct Message {
        bool relaysActions[RELAYS_COUNT];
    };

    struct HelloMessage {
        char msg[10] = "hello";
    };

    const int switchesPins[SWITCHES_COUNT] = {32, 33, 25, 26, 27, 14, 12, 23, 22, 21, 19, 18, 5, 17};
    const int statusLed = 16;
    
    uint8_t knownPeers[MAX_PEERS][6];
    int peerCount = 0;
    Message currentData;

    static ESPNowTransmitter* instance;
    
    static void OnDataRecv(const uint8_t* mac, const uint8_t* data, int len);
    static void OnDataSent(const uint8_t* mac, esp_now_send_status_t status);
    
    bool isPeerKnown(const uint8_t* mac);
    void addPeer(const uint8_t* mac);
    void readMacAddress();
    void readSwitches();

public:
    ESPNowTransmitter();
    void init();
    void update();
};