#ifndef DisplayWaveshare_H
#define DisplayWaveshare_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Vooruit-declaratie van Entity om cirkel-afhankelijkheden te voorkomen
struct Entity {
    float x, z;
    bool active;
    int lastSx, lastSy, lastSize;
};

class DisplayWaveshare {
public:
    DisplayWaveshare(int cs, int dc, int rst);
    void begin();
    void clearScreen();
    void gameOverScreen(int killCount);
    void drawEnemy(int x, int y, int size, int lastX, int lastY, int lastSize);
    // void drawShootEffect();
    void triggerShootEffect();
    void updateShootEffect();
    void drawGrid(float playerX, float playerZ, float oldPlayerX, float oldPlayerZ);
    void drawEnemyIndicator(int side); // boven en onder voor de toekomst mss er bij
    void drawReloadScreen();
    void drawPauseScreen();

    // Tekent statische UI elementen zoals het vizier
    void drawUI(int killCount);

private:
    Adafruit_ILI9341 _tft;
    const int _screenWidth = 320;
    const int _screenHeight = 240;
    int lastIndicatorSide = -1; // -1 = geen, 0 = links, 1 = rechts, 2 = onder, 3 = boven
};

#endif