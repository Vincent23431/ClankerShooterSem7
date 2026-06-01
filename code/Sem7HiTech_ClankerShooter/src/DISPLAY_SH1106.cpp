#include "DISPLAY_SH1106.h"

void DISPLAY_SH1106::begin() {
    u8g2.begin();
}

void DISPLAY_SH1106::clearBuffer() {
    u8g2.clearBuffer();
}
void DISPLAY_SH1106::drawHUD(int ammo, int maxAmmo, int kills) {
    u8g2.clearBuffer();					// Wis het kleine scherm
    u8g2.setFont(u8g2_font_6x10_tf); 	// Compact font

    // Teken tekst
    u8g2.setCursor(0, 15);
    u8g2.print("AMMO: ");
    u8g2.print(ammo);
    u8g2.print("/");
    u8g2.print(maxAmmo);

    u8g2.setCursor(0, 30);
    u8g2.print("KILLS: ");
    u8g2.print(kills);

    // ammo-balkje onderin
    int barWidth = (ammo * 120) / maxAmmo; 
    u8g2.drawFrame(0, 50, 122, 10);      // Omlijning
    u8g2.drawBox(1, 51, barWidth, 8);   // De gevulde balk

    u8g2.sendBuffer();					// Stuur naar scherm
}