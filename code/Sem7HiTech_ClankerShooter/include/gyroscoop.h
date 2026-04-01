#ifndef gyroscoop_H
#define gyroscoop_H

#include <Arduino_LSM6DSOX.h>

class gyroscoop {
public:
    gyroscoop();
    bool begin();
    void update();
    void calibrate(int samples = 200);
    
    float getAngle() const { return _angle; }
    void resetAngle() { _angle = 0.0f; }

private:
    float _angle;       // Current angle in radians
    float _gyroBiasZ;   // The "drift" offset
    unsigned long _lastUpdate;
};

#endif