#include <unity.h>

void setUp(void) {
    // Code hier runt VÓÓR elke test
}

void tearDown(void) {
    // Code hier runt NA elke test
}

// Onze test
void test_altijd_waar(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

int main(int argc, char **argv) {
    UNITY_BEGIN(); //
    
    RUN_TEST(test_altijd_waar); //
    
    return UNITY_END(); //
}