#ifndef RADAR_H
#define RADAR_H

#include <Arduino.h>
#include "Clanker.h"

class Radar {
public:
    Radar(int maxEnemies);

    int getClosestEnemySide(float pX, float pZ, float pAngle, Clanker* enemies, bool* activeArray);
        
private:
    int _maxEnemies;
};

#endif