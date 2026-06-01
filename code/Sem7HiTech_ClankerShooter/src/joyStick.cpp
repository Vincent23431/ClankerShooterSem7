#include "JoyStick.h"

JoyStick::JoyStick(int pinX, int pinY) : _pinX(pinX), _pinY(pinY) {}

void JoyStick::begin() {
    pinMode(_pinX, INPUT);
    pinMode(_pinY, INPUT);
}

float JoyStick::getX() {
    int val = analogRead(_pinX);
    if (abs(val - _center) < _deadzone) return 0.0f;
    return (val - _center) / 2048.0f;
}

float JoyStick::getY() {
    int val = analogRead(_pinY);
    if (abs(val - _center) < _deadzone) return 0.0f;
    return (val - _center) / 2048.0f; 
}