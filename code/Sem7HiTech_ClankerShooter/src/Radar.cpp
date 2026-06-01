#include "Radar.h"
#include <math.h>

Radar::Radar(int maxEnemies) {
    _maxEnemies = maxEnemies;
}

int Radar::getClosestEnemySide(float pX, float pZ, float pAngle, Clanker* enemies, bool* activeArray) {
    float minDistance = 999999.0f;
    int closestIdx = -1;

    for (int i = 0; i < _maxEnemies; i++) {
        if (!activeArray[i] || enemies[i].isDead()) continue;
        float dx = enemies[i].getX() - pX;
        float dz = enemies[i].getZ() - pZ;
        float dist = sqrt(dx * dx + dz * dz);
        if (dist < minDistance) {
            minDistance = dist;
            closestIdx = i;
        }
    }

    if (closestIdx == -1) return -1;

    float dx = enemies[closestIdx].getX() - pX;
    float dz = enemies[closestIdx].getZ() - pZ;
    float targetAngle = atan2f(dx, dz);
    float diff = targetAngle - pAngle;

    // Normalisatie naar -PI tot PI
    while (diff >  PI) diff -= 2.0f * PI;
    while (diff < -PI) diff += 2.0f * PI;

    float fovThreshold = 0.50f; // lets iets ruimer voor rustiger beeld

    if (abs(diff) < fovThreshold) return -1; // In beeld

    // Bepaling met duidelijke grenzen
    if (diff > fovThreshold && diff < 2.4f) {
        return 0; // Links
    } 
    else if (diff < -fovThreshold && diff > -2.4f) {
        return 1; // Rechts
    } 
    else {
        return 2; // Achter (alles groter dan 2.4 of kleiner dan -2.4)
    }
}