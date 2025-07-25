#pragma once
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define RELAYS_COUNT 14

class ESPNowReceiver {
private:
    struct Message {
        bool relaysActions[16];
    };

    struct HelloMessage {
        char msg[10] = "hello";
    };

    const int relaysPins[RELAYS_COUNT] = {32, 33, 25, 26, 27, 14, 12, 17, 5, 18, 19, 21, 22, 23};
    const uint8_t masterMac[6] = {0xEC, 0x64, 0xC9, 0x85, 0xF7, 0x50};
    Message currentData;

    static ESPNowReceiver* instance;
    static void OnDataRecv(const uint8_t* mac, const uint8_t* data, int len);
    
    void sendHello();

public:
    ESPNowReceiver();
    void init();
    void update();
};