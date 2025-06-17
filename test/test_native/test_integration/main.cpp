#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void testValidCardFlow();
void testInvalidCardFlowFirstAttempt();
void testInvalidCardFlowSecondAttempt();
void testInvalidCardFlowThirdAttempt();
void testExponentialBackoff();
void testMaxInvalidAttempts();
void testWaitingSoundAfter10Seconds();
void testRelayTimingAccuracy();

// setUp and tearDown are defined in test_helpers.cpp

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testValidCardFlow);
    RUN_TEST(testInvalidCardFlowFirstAttempt);
    RUN_TEST(testInvalidCardFlowSecondAttempt);
    RUN_TEST(testInvalidCardFlowThirdAttempt);
    RUN_TEST(testExponentialBackoff);
    RUN_TEST(testMaxInvalidAttempts);
    RUN_TEST(testWaitingSoundAfter10Seconds);
    RUN_TEST(testRelayTimingAccuracy);

    return UNITY_END();
}
