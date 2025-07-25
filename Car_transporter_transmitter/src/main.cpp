#include "Transmitter.h"

ESPNowTransmitter transmitter;

void setup() {
    transmitter.init();
}

void loop() {
    transmitter.update();
}