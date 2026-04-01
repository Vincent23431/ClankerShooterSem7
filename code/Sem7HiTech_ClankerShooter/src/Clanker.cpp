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

    // Camera rotatie (radialen)
    float rotX = dx * cos(-pAngle) - dz * sin(-pAngle);
    float rotZ = dx * sin(-pAngle) + dz * cos(-pAngle);

    if (rotZ > 5.0f) { // Alleen zichtbaar als het voor de speler is
        sp.visible = true;
        sp.size = (int)(2000 / rotZ); // Hoe dichterbij (kleine rotZ), hoe groter het vierkant
        sp.x = 160 + (int)(rotX * 250 / rotZ); // Perspectief: dingen aan de zijkant vliegen sneller weg
        sp.y = 120;
    } else {
        sp.visible = false;
    }
    return sp;
}