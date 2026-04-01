#ifndef __Button__
#define __Button__

#include <Arduino.h>
#include "IButton.h"

class Button : public IButton {
private:
    const int pin;
    bool currentState;
    bool lastState;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
    bool pullUp;

public:
    Button(int buttonPin, unsigned long delay, bool stateOfPullUp);
    bool readButton();
};

#endif
