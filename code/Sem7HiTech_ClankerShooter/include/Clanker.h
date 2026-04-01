#ifndef CLANKER_H
#define CLANKER_H

#include <Arduino.h>

struct ScreenPos {
    int x, y, size;
    bool visible;
};

class Clanker {
public:
    Clanker(float startX, float startZ);
    
    // Beweging
    void update(float pX, float pZ);
    
    // Berekening van 3D wereld naar 2D scherm
    ScreenPos calculateProjection(float pX, float pZ, float pAngle);

    // game logic
    void takeDamage(int amount) { _hp -= amount; }
    bool isDead() { return _hp <= 0; }
    int getHp() { return _hp; }
    float getX() { return _x; }
    float getZ() { return _z; }
    void reset(float startX, float startZ);

    // Variabelen om de vorige tekenbeurt te onthouden (voor het wissen)
    int lastSx, lastSy, lastSize;

private:
    float _x, _z;
    const float _speed = 0.4f; 
    int _hp = 100;
};

#endif