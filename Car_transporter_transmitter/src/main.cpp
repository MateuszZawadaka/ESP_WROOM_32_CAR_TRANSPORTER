#include "Transmitter.h"
ESPNowTransmitter transmitter;

void setup() {
    vTaskDelay(500/ portTICK_RATE_MS);
    transmitter.init();
    vTaskDelay(500/ portTICK_RATE_MS);
}

void loop() {
    transmitter.update();
    vTaskDelay(500/ portTICK_RATE_MS);
}