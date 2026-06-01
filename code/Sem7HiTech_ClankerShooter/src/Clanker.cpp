#include "Clanker.h"
#include <math.h>

Clanker::Clanker(float startX, float startZ) : _x(startX), _z(startZ) {
    lastSx = 0; lastSy = 0; lastSize = 0;
}

void Clanker::update(float pX, float pZ) {
    // Beweeg langzaam richting de speler
    float dx = pX - _x;
    float dz = pZ - _z;
    float dist = sqrt(dx*dx + dz*dz);

    if (dist > 5.0f) { 
        _x += (dx / dist) * _speed; // Stapje richting de speler op de X-as
        _z += (dz / dist) * _speed;
    }
}

void Clanker::reset(float startX, float startZ) {
    _x = startX;
    _z = startZ;
    _hp = 100;
    
    // Cruciaal: vergeet de oude schermpositie
    lastSize = 0; 
    lastSx = 0;
    lastSy = 0;
}

ScreenPos Clanker::calculateProjection(float pX, float pZ, float pAngle) {
    ScreenPos sp;
    float dx = _x - pX;
    float dz = _z - pZ;

    float rotX = dx * cos(-pAngle) - dz * sin(-pAngle);
    float rotZ = dx * sin(-pAngle) + dz * cos(-pAngle);

    // Door rotZ + een getal te doen (bijv. 10), wordt de groei minder extreem
    if (rotZ > 2.0f) { 
        sp.visible = true;
        sp.size = (int)(1800 / (rotZ + 8.0f));
        sp.x = 160 + (int)(rotX * 200 / rotZ);
        sp.y = 120;
    } else {
        sp.visible = false;
    }

    sp.size = (int)(6000 / (rotZ + 8.0f));

    // Serial.print("rotZ=");
    // Serial.print(rotZ);
    // Serial.print(" size=");
    // Serial.println(sp.size);
    return sp;
}

bool Clanker::checkHit(int hx, int hy, float pX, float pZ, float pAngle) {
    // 1. Bereken waar de vijand op dit moment op het scherm staat
    ScreenPos pos = calculateProjection(pX, pZ, pAngle);

    // 2. Als de vijand niet in beeld is, kun je hem nooit raken
    if (!pos.visible) return false;

    // 3. De Hitbox bepalen
    // Omdat we een sprite van 16x16 tekenen, gebruiken we een vaste marge.
    // We maken de hitbox iets ruimer (bijv. 10 pixels naar elke kant) 
    // zodat het spel niet frustrerend moeilijk wordt.
    int hitboxSize = 10; 

    if (hx > (pos.x - hitboxSize) && hx < (pos.x + hitboxSize) &&
        hy > (pos.y - hitboxSize) && hy < (pos.y + hitboxSize)) {
        return true;
    }

    return false;
}