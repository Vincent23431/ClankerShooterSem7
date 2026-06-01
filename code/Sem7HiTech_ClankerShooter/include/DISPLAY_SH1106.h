#ifndef DISPLAY_SH1106_H
#define DISPLAY_SH1106_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <math.h>

class DISPLAY_SH1106 { // Ensure public inheritance
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;  // Use the specific display type

public:
    DISPLAY_SH1106() : u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE) {}
    void begin();
    void clearBuffer();
    void drawHUD(int ammo, int maxAmmo, int kills);
};

#endif // DISPLAY_SH1106_H