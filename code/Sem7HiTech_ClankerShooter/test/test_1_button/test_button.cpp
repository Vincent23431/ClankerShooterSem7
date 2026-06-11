#include <unity.h>
#include "arduino_fake.h"

#include "../../include/Button.h"
#include "../../src/Button.cpp"

// Helperfunctie om de knop eerst in een stabiele HIGH-staat te krijgen.
// Dit is nodig omdat de constructor 'currentState = false' (LOW) initialiseert.
void helper_synchronize_button_to_high(Button& btn) {
    fakePinState = HIGH;
    fakeMillis = 0;
    btn.readButton(); 
    fakeMillis = 60; // Voorbij de 50ms debounce
    btn.readButton(); // currentState is nu HIGH (1)
}

void setUp(void) {
    fakeMillis = 0;
    fakePinState = HIGH;
}

void tearDown(void) {}

// =========================================================================
// TEST 1: De succesvolle in drukken 
// =========================================================================
void test_button_press_registers_after_delay(void) {
    Button testButton(2, 50, true);
    helper_synchronize_button_to_high(testButton);

    // Knop indrukken op t = 70
    fakePinState = LOW;
    fakeMillis = 70;
    testButton.readButton(); 

    // Wachten tot voorbij debounce window (t = 130)
    fakeMillis = 130;
    bool result = testButton.readButton(); 

    TEST_ASSERT_EQUAL(true, result);
}

// =========================================================================
// TEST 2: Ruis / Storing negeren (Glitch filter)
// =========================================================================
void test_button_should_ignore_short_glitches(void) {
    Button testButton(2, 50, true);
    helper_synchronize_button_to_high(testButton);

    // Storing: Knop schiet heel even naar LOW op t = 70
    fakePinState = LOW;
    fakeMillis = 70;
    testButton.readButton();

    // Na 20ms schiet de pin alweer terug naar HIGH (storing voorbij, korter dan 50ms)
    fakePinState = HIGH;
    fakeMillis = 90;
    bool result = testButton.readButton();

    // Dit mag absoluut GEEN geldige knopdruk zijn geweest
    TEST_ASSERT_EQUAL(false, result);
}

// =========================================================================
// TEST 3: Lang ingedrukt houden geeft maar één puls (Flankdetectie)
// =========================================================================
void test_button_held_down_should_only_return_true_once(void) {
    Button testButton(2, 50, true);
    helper_synchronize_button_to_high(testButton);

    // Knop indrukken op t = 70
    fakePinState = LOW;
    fakeMillis = 70;
    testButton.readButton();

    // Na 60ms (op t = 130) checken we: dit moet TRUE zijn (eerste registratie)
    fakeMillis = 130;
    TEST_ASSERT_EQUAL(true, testButton.readButton());

    // De vinger blijft op de knop! We checken 100ms later weer (op t = 230)
    fakeMillis = 230;
    bool resultSecondTime = testButton.readButton();

    // Omdat de knop nog steeds ingedrukt is, moet hij nu FALSE retourneren (geen autofire)
    TEST_ASSERT_EQUAL(false, resultSecondTime);
}

// =========================================================================
// TEST 4: Loslaten en opnieuw indrukken (Volgende klik)
// =========================================================================
void test_button_can_be_pressed_again_after_release(void) {
    Button testButton(2, 50, true);
    helper_synchronize_button_to_high(testButton);

    // --- EERSTE KLIK ---
    fakePinState = LOW; fakeMillis = 70; testButton.readButton();
    fakeMillis = 130; testButton.readButton(); // Eerste klik verwerkt

    // --- LOSLATEN ---
    fakePinState = HIGH;
    fakeMillis = 140;
    testButton.readButton(); // Start debounce voor loslaten
    
    fakeMillis = 200;
    testButton.readButton(); // Knop is nu ook softwarematig weer stabiel HIGH

    // --- TWEEDE KLIK ---
    fakePinState = LOW;
    fakeMillis = 210;
    testButton.readButton(); // Start debounce voor tweede klik

    fakeMillis = 270;
    bool resultSecondPress = testButton.readButton();

    // De tweede klik moet ook netjes TRUE opleveren!
    TEST_ASSERT_EQUAL(true, resultSecondPress);
}

// =========================================================================
// MAIN RUNNER
// =========================================================================
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_button_press_registers_after_delay);
    RUN_TEST(test_button_should_ignore_short_glitches);
    RUN_TEST(test_button_held_down_should_only_return_true_once);
    RUN_TEST(test_button_can_be_pressed_again_after_release);
    return UNITY_END();
}