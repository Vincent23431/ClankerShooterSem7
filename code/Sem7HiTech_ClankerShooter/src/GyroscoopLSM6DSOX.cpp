#include "GyroscoopLSM6DSOX.h"

GyroscoopLSM6DSOX::GyroscoopLSM6DSOX() : _angle(0.0f), _pitch(0.0f), _gyroBiasZ(0.0f), _lastUpdate(0) {}

bool GyroscoopLSM6DSOX::begin() {
    if (!IMU.begin()) return false;
    calibrate();
    _lastUpdate = micros();
    return true;
}

void GyroscoopLSM6DSOX::calibrate(int samples) {
    float sumZ = 0;
    for (int i = 0; i < samples; i++) {
        if (IMU.gyroscopeAvailable()) {
            float x, y, z;
            IMU.readGyroscope(x, y, z);
            sumZ += z;
        }
        delay(5);
    }
    _gyroBiasZ = sumZ / samples;
}

void GyroscoopLSM6DSOX::resetGyro() {
    _angle = 0.0f;
    _pitch = 0.0f;
    _gyroBiasZ = 0.0f;
    calibrate();
}

void GyroscoopLSM6DSOX::update() {
    float gx, gy, gz;
    float ax, ay, az;

    // 1. Gyroscoop voor Yaw (ronddraaien)
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
        unsigned long currentTime = micros();
        float dt = (currentTime - _lastUpdate) / 1000000.0f;
        _lastUpdate = currentTime;

        float correctedZ = gz - _gyroBiasZ;
        if (abs(correctedZ) < 0.5f) correctedZ = 0; 
        _angle += (correctedZ * (M_PI / 180.0f)) * dt;
    }

    // 2. Accelerometer voor Pitch (omhoog/omlaag kijken)
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
        
        // Bereken de hoek in graden. 
        // Als je de sensor plat houdt, is az = 1g. 
        // Als je naar de vloer kantelt, verandert ay en az.
        _pitch = atan2(ay, az) * (180.0f / M_PI);
    }
}