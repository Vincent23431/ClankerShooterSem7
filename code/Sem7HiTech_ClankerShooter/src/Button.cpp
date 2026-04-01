#include "Button.h"
#include <Arduino.h>

Button::Button(int buttonPin, unsigned long delay, bool stateOfPullUp) : pin(buttonPin), debounceDelay(delay), pullUp(stateOfPullUp) {
    currentState = false;
    lastState = HIGH;
    lastDebounceTime = 0;
    
    if (stateOfPullUp){
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT);
    }
}

bool Button::readButton() {
    int reading = digitalRead(pin);
    
    if (reading != lastState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != currentState) {
            currentState = reading;

            if (currentState == LOW) {
                lastState = reading;
                return true;
            }
        }
    }

    lastState = reading;
    return false;
}
