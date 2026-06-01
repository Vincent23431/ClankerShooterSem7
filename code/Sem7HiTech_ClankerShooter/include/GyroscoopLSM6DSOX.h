#ifndef GyroscoopLSM6DSOX_H
#define GyroscoopLSM6DSOX_H

#include <Arduino_LSM6DSOX.h>

class GyroscoopLSM6DSOX {
public:
    GyroscoopLSM6DSOX();
    bool begin();
    void update();
    void calibrate(int samples = 200);
    void resetGyro();
    float getAngle() const { return _angle; } // zijwaards hoek
    float getPitch() const { return _pitch; } // hoogte hoek

private:
    float _angle;       // Current angle in radians
    float _pitch; // Voor omhoog/omlaag
    float _gyroBiasZ;   // The "drift" offset
    unsigned long _lastUpdate;
};

#endif