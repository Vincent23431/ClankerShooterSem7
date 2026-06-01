#include "DisplayWaveshare.h"
#include <math.h>
#include "bitmaps.h"

DisplayWaveshare::DisplayWaveshare(int cs, int dc, int rst) : _tft(cs, dc, rst) {}

void DisplayWaveshare::begin() {
    _tft.begin();
    _tft.setSPISpeed(27000000);  // 40 MHz
    _tft.setRotation(3);
    _tft.fillScreen(ILI9341_BLACK);
}

void DisplayWaveshare::clearScreen() {
    _tft.fillScreen(ILI9341_BLACK);
}

void DisplayWaveshare::drawPauseScreen() {
    _tft.fillScreen(ILI9341_BLACK);
    _tft.setTextSize(2);
    _tft.setTextColor(ILI9341_YELLOW);
    _tft.setCursor(60, 100);
    _tft.println("Game Paused");
}

void DisplayWaveshare::gameOverScreen(int killCount) {
    // Maak scherm zwart
    _tft.fillScreen(ILI9341_BLACK);

    // Titel
    _tft.setTextSize(3);
    _tft.setTextColor(ILI9341_RED);
    _tft.setCursor(40, 80);
    _tft.println("GAME OVER");

    // Subtekst
    _tft.setTextSize(2);
    _tft.setTextColor(ILI9341_WHITE);
    _tft.setCursor(50, 160);
    _tft.print("Kills: ");
    _tft.println(killCount);
}

void DisplayWaveshare::drawReloadScreen() {
    _tft.fillScreen(ILI9341_BLACK);
    _tft.setTextSize(2);
    _tft.setTextColor(ILI9341_YELLOW);
    _tft.setCursor(60, 100);
    _tft.println("Reloading...");
}

void DisplayWaveshare::drawGrid(float playerX, float playerZ, float oldPlayerX, float oldPlayerZ) {
    static int lastOffset = 0; 

    int spacing = 40; // distance between grid lines
    int offset = (int)playerZ % spacing;
    int offset2 = (int)oldPlayerZ % spacing;

    for (int y = 120; y < 240; y += spacing) {
        int lineY = y + offset;
        int linyY2 = y + offset2;

        if (linyY2 >= 120 && linyY2 < 240) {
            _tft.drawFastHLine(0, linyY2, 320, ILI9341_BLACK);
        }

        if (lineY >= 120 && lineY < 240) {
            _tft.drawFastHLine(0, lineY, 320, ILI9341_DARKGREY);
        }
    }
}

void DisplayWaveshare::drawUI(int killCount) {
    _tft.drawFastHLine(155, 120, 10, ILI9341_GREEN);
    _tft.drawFastVLine(160, 115, 10, ILI9341_GREEN);
}

void DisplayWaveshare::drawEnemy(int x, int y, int size, int lastX, int lastY, int lastSize) {
    int drawSize = size;  // geen constraining hier, we willen dat het groter kan worden en kleiner dan 16
    int half = drawSize / 2;

    if (lastSize > 0) {
        int lastDrawSize = constrain(lastSize, 16, 64);
        int lastHalf = lastDrawSize / 2;
        _tft.fillRect(lastX - lastHalf, lastY - lastHalf, lastDrawSize, lastDrawSize, ILI9341_BLACK);
    }

    if (drawSize > 0) {
        float scale = drawSize / 16.0f;

        for (int sy = 0; sy < 16; sy++) {
            for (int sx = 0; sx < 16; sx++) {

                uint16_t color = pgm_read_word(&clanker_16x16[sy * 16 + sx]);

                if (color != 0x0000) {

                    int px = x - half + (int)(sx * scale);
                    int py = y - half + (int)(sy * scale);

                    int pixelSize = max(1, (int)ceil(scale));

                    _tft.fillRect(
                        px,
                        py,
                        pixelSize,
                        pixelSize,
                        color
                    );
                }
            }
        }
    }
}

bool shootFlash = false;
unsigned long shootTime = 0;
void DisplayWaveshare::triggerShootEffect() {
    shootFlash = true;
    shootTime = millis();
}

void DisplayWaveshare::updateShootEffect() {
    if (!shootFlash) return;

    if (millis() - shootTime < 20) {
        _tft.drawCircle(160, 120, 20, ILI9341_WHITE);
    } else {
        _tft.drawCircle(160, 120, 20, ILI9341_BLACK);
        shootFlash = false;
    }
}

void DisplayWaveshare::drawEnemyIndicator(int side) {
    // 1. Altijd de lastIndicatorSide updaten, maar we gaan 'side' gebruiken voor de logica
    if (side == lastIndicatorSide) return;

    // 2. EXTRA ROBUUST WISSEN
    // We maken de wis-zone iets groter dan de teken-zone (3 pixels ipv 2)
    // Dit ruimt de "half rode strepen" op die ontstaan bij snelle bewegingen.
    if (lastIndicatorSide != -1) {
        _tft.fillRect(0, 0, 3, 240, ILI9341_BLACK);   // Wis Links
        _tft.fillRect(317, 0, 3, 240, ILI9341_BLACK); // Wis Rechts
        _tft.fillRect(0, 237, 320, 3, ILI9341_BLACK); // Wis Onder
        _tft.fillRect(0, 0, 320, 3, ILI9341_BLACK);   // Wis Boven
    }

    // 3. Teken de nieuwe indicator
    if (side == 0) { // Links
        _tft.fillRect(0, 0, 2, 240, ILI9341_RED);
    } 
    else if (side == 1) { // Rechts
        _tft.fillRect(318, 0, 2, 240, ILI9341_RED);
    } 
    else if (side == 2) { // Achter
        _tft.fillRect(0, 238, 320, 2, ILI9341_RED);
    }
    else if (side == 3) { // Boven
        _tft.fillRect(0, 0, 320, 2, ILI9341_RED);
    }

    lastIndicatorSide = side;
}
