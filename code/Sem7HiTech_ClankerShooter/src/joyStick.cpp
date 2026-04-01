#include "joyStick.h"

joyStick::joyStick(int pinX, int pinY) : _pinX(pinX), _pinY(pinY) {}

void joyStick::begin() {
    pinMode(_pinX, INPUT);
    pinMode(_pinY, INPUT);
}

float joyStick::getX() {
    int val = analogRead(_pinX);
    if (abs(val - _center) < _deadzone) return 0.0f;
    return (val - _center) / 2048.0f;
}

float joyStick::getY() {
    int val = analogRead(_pinY);
    if (abs(val - _center) < _deadzone) return 0.0f;
    return (val - _center) / 2048.0f; 
}