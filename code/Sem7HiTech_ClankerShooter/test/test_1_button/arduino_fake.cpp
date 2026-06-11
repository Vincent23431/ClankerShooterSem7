#include "arduino_fake.h"

unsigned long fakeMillis = 0;
int fakePinState = HIGH;

unsigned long millis() {
    return fakeMillis;
}

int digitalRead(int pin) {
    return fakePinState;
}

void pinMode(int pin, int mode) {
    (void)pin;
    (void)mode;
}