#include <test_helpers.h>  // Provides setUp() and tearDown()
#include <unity.h>
#include "test_integration.h"  // Include the test function declarations

int main(void) {
    UNITY_BEGIN();

    // Run your test functions
    RUN_TEST(testValidCardFlow);
    RUN_TEST(testInvalidCardFlowFirstAttempt);
    RUN_TEST(testInvalidCardFlowSecondAttempt);
    RUN_TEST(testInvalidCardFlowThirdAttempt);
    RUN_TEST(testExponentialBackoff);
    RUN_TEST(testMaxInvalidAttempts);
    RUN_TEST(testWaitingSoundAfter10Seconds);
    RUN_TEST(testRelayTimingAccuracy);
    RUN_TEST(testSequentialRelayActivation);

    return UNITY_END();
}
