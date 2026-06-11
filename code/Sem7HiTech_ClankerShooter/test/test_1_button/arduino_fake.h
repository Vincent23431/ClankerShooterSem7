#pragma once

// =======================
// Arduino constants
// =======================
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 1

// =======================
// Fake time + input
// =======================
extern unsigned long fakeMillis;
extern int fakePinState;

// =======================
// Fake Arduino functions
// =======================
unsigned long millis();
int digitalRead(int pin);
void pinMode(int pin, int mode);