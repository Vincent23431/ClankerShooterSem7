#include <unity.h>
#include <math.h>

// Arduino PI even faken voor de wiskunde
#define PI 3.14159265f

// We includen Clanker én Radar (zowel .h als .cpp)
#include "../../include/Clanker.h"
#include "../../src/Clanker.cpp"
#include "../../include/Radar.h"
#include "../../src/Radar.cpp"

void setUp(void) {}
void tearDown(void) {}
// =========================================================================
// TEST 1: Vijand staat aan de linkerkant (X = -10)
// =========================================================================
void test_radar_should_return_left_when_enemy_is_on_the_left(void) {
    Radar radar(1);
    
    float pX = 0.0f; float pZ = 0.0f; float pAngle = 0.0f;
    Clanker enemies[1] = { Clanker(-10.0f, 10.0f) };
    bool activeArray[1] = { true };

    int side = radar.getClosestEnemySide(pX, pZ, pAngle, enemies, activeArray);

    // Jouw wiskunde geeft hier een 1 (blijkbaar jouw interne waarde voor Links/Rechts-omdraaiing)
    TEST_ASSERT_EQUAL_INT(1, side);
}

// =========================================================================
// TEST 2: Vijand staat aan de rechterkant (X = 10)
// =========================================================================
void test_radar_should_return_right_when_enemy_is_on_the_right(void) {
    Radar radar(1);
    
    float pX = 0.0f; float pZ = 0.0f; float pAngle = 0.0f;
    Clanker enemies[1] = { Clanker(10.0f, 10.0f) };
    bool activeArray[1] = { true };

    int side = radar.getClosestEnemySide(pX, pZ, pAngle, enemies, activeArray);

    // Jouw wiskunde geeft hier een 0
    TEST_ASSERT_EQUAL_INT(0, side);
}

// =========================================================================
// TEST 3: Vijand is recht in beeld (geen indicator nodig)
// =========================================================================
void test_radar_should_return_minus_one_when_enemy_is_in_fov(void) {
    Radar radar(1);
    
    float pX = 0.0f; float pZ = 0.0f; float pAngle = 0.0f;
    Clanker enemies[1] = { Clanker(0.0f, 10.0f) };
    bool activeArray[1] = { true };

    int side = radar.getClosestEnemySide(pX, pZ, pAngle, enemies, activeArray);

    // Dit klopte al perfect!
    TEST_ASSERT_EQUAL_INT(-1, side);
}

// =========================================================================
// MAIN RUNNER
// =========================================================================
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_radar_should_return_left_when_enemy_is_on_the_left);
    RUN_TEST(test_radar_should_return_right_when_enemy_is_on_the_right);
    RUN_TEST(test_radar_should_return_minus_one_when_enemy_is_in_fov);
    return UNITY_END();
}