#include "display.h"
#include <math.h>

display::display(int cs, int dc, int rst) : _tft(cs, dc, rst) {}

void display::begin() {
    _tft.begin();
    _tft.setRotation(3);
    _tft.fillScreen(ILI9341_BLACK);
}

void display::clearScreen() {
    _tft.fillScreen(ILI9341_BLACK);
}

void display::gameOverScreen() {
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
    _tft.setCursor(50, 130);
    _tft.println("Je bent geraakt!");
}

void display::drawUI() {
    _tft.drawFastHLine(155, 120, 10, ILI9341_GREEN);
    _tft.drawFastVLine(160, 115, 10, ILI9341_GREEN);
}

void display::drawEnemy(int x, int y, int size, int lastX, int lastY, int lastSize) {
    // Wis vorige frame
    if (lastSize > 0) {
        _tft.fillRect(lastX - lastSize/2, lastY - lastSize/2, lastSize, lastSize, ILI9341_BLACK);
    }
    // Teken nieuwe frame
    if (size > 0) {
        _tft.fillRect(x - size/2, y - size/2, size, size, ILI9341_RED);
    }
}

void display::drawShootEffect() {
    // Teken een wit kruis of flits in het midden
    _tft.drawCircle(160, 120, 20, ILI9341_WHITE);
    delay(20); // Hele korte flits
    _tft.drawCircle(160, 120, 20, ILI9341_BLACK); // Wis flits
}