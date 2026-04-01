#ifndef joyStick_H
#define joyStick_H

// #include <Arduino_LSM6DSOX.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class joyStick {
public:
    joyStick(int pinX, int pinY);
    void begin();
    
    // Returns values from -1.0 to 1.0
    float getX();
    float getY();
    
    bool isPressed(int threshold = 3000); // For forward detection

private:
    int _pinX;
    int _pinY;
    const int _center = 2048; // Standard for 12-bit ADC (ESP32)
    const int _deadzone = 200; 
};

#endif