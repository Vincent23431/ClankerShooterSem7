#include <unity.h>
#include <math.h>
#include "../../include/Clanker.h"
#include "../../src/Clanker.cpp"

void setUp(void) {
    // Wordt uitgevoerd vóór elke test
}

void tearDown(void) {
    // Wordt uitgevoerd na elke test
}

// =========================================================================
// TEST 1: Beweegt de vijand naar de speler toe?
// =========================================================================
void test_clanker_should_move_towards_player(void) {
    // Vijand staat op X=0, Z=10. Speler staat op X=0, Z=0.
    Clanker enemy(0.0f, 10.0f);
    
    // Roep update aan met de positie van de speler (0, 0)
    enemy.update(0.0f, 0.0f);

    // De vijand heeft een _speed van 1.0f. 
    // Hij moet zich over de Z-as nu verplaatst hebben van 10.0f naar 9.0f.
    TEST_ASSERT_EQUAL_FLOAT(0.0f, enemy.getX());
    TEST_ASSERT_EQUAL_FLOAT(9.0f, enemy.getZ());
}

// =========================================================================
// TEST 2: Schade logica en doodgaan
// =========================================================================
void test_clanker_damage_and_death_logic(void) {
    Clanker enemy(0.0f, 10.0f);

    // Check begin HP
    TEST_ASSERT_EQUAL_INT(100, enemy.getHp());
    TEST_ASSERT_EQUAL(false, enemy.isDead());

    // Geef 40 schade
    enemy.takeDamage(40);
    TEST_ASSERT_EQUAL_INT(60, enemy.getHp());
    TEST_ASSERT_EQUAL(false, enemy.isDead());

    // Geef nog eens 60 schade (totaal 100)
    enemy.takeDamage(60);
    TEST_ASSERT_EQUAL(0, enemy.getHp());
    TEST_ASSERT_EQUAL(true, enemy.isDead());
}

// =========================================================================
// TEST 3: Hitbox check (Raken we de vijand als we schieten?)
// =========================================================================
void test_clanker_should_register_hit_when_shot_in_center(void) {
    // Vijand staat recht voor de speler uit op Z=10 (X=0)
    Clanker enemy(0.0f, 10.0f);
    
    // Speler staat op (0,0) en kijkt recht vooruit (angle = 0.0f)
    float playerX = 0.0f;
    float playerZ = 0.0f;
    float playerAngle = 0.0f;

    // We schieten exact in het midden van het scherm (160, 120)
    bool hit = enemy.checkHit(160, 120, playerX, playerZ, playerAngle);

    // Omdat de vijand recht voor onze neus staat, moet dit TRUE opleveren!
    TEST_ASSERT_EQUAL(true, hit);
}

// =========================================================================
// MAIN RUNNER
// =========================================================================
int main(int argc, char **argv) {
    UNITY_BEGIN(); //
    RUN_TEST(test_clanker_should_move_towards_player); //
    RUN_TEST(test_clanker_damage_and_death_logic); //
    RUN_TEST(test_clanker_should_register_hit_when_shot_in_center); //
    return UNITY_END(); //
}