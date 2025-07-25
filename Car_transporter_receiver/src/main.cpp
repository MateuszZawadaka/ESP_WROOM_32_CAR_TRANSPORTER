#include "Receiver.h"

ESPNowReceiver receiver;

void setup() {
    receiver.init();
}

void loop() {
    receiver.update();
}