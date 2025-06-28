#include <test_helpers.h>  // Provides setUp() and tearDown()
#include <unity.h>
#include "test_relay_controller.h"  // Include the test function declarations

int main(void) {
    UNITY_BEGIN();

    // Run your test functions
    RUN_TEST(testRelayInitialState);
    RUN_TEST(testSetSingleRelay);
    RUN_TEST(testSetAllRelays);
    RUN_TEST(testInvalidRelayNumber);
    RUN_TEST(testRelayStateTransitions);
    RUN_TEST(testRapidRelaySwitching);
    RUN_TEST(testSequentialRelayOperations);

    return UNITY_END();
}
