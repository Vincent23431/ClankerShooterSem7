#ifndef display_H
#define display_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Vooruit-declaratie van Entity om cirkel-afhankelijkheden te voorkomen
struct Entity {
    float x, z;
    bool active;
    int lastSx, lastSy, lastSize;
};

class display {
public:
    display(int cs, int dc, int rst);
    void begin();
    void clearScreen();
    void gameOverScreen();
    
    void drawEnemy(int x, int y, int size, int lastX, int lastY, int lastSize);
    void drawShootEffect();

    // Tekent statische UI elementen zoals het vizier
    void drawUI();

private:
    Adafruit_ILI9341 _tft;
    const int _screenWidth = 320;
    const int _screenHeight = 240;
};

#endif