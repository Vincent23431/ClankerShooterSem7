#include "gyroscoop.h"

gyroscoop::gyroscoop() : _angle(0.0f), _gyroBiasZ(0.0f), _lastUpdate(0) {}

bool gyroscoop::begin() {
    if (!IMU.begin()) return false;
    calibrate();
    _lastUpdate = micros();
    return true;
}

void gyroscoop::calibrate(int samples) {
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

void gyroscoop::update() {
    if (IMU.gyroscopeAvailable()) {
        float gx, gy, gz;
        IMU.readGyroscope(gx, gy, gz);

        // Calculate time delta
        unsigned long currentTime = micros();
        float dt = (currentTime - _lastUpdate) / 1000000.0f;
        _lastUpdate = currentTime;

        // Apply bias and deadzone to reduce drift
        float correctedZ = gz - _gyroBiasZ;
        if (abs(correctedZ) < 0.5f) correctedZ = 0; 

        // Integrate: angle = angle + (angular_velocity * time)
        // Convert to Radians: degrees * (PI / 180)
        _angle += (correctedZ * (PI / 180.0f)) * dt;
    }
}