#ifndef TEST_INTEGRATION_H
#define TEST_INTEGRATION_H

#include <unity.h>  // Include Unity for TEST_ASSERT_... macros if needed in header

// Declare your test functions here
void testValidCardFlow();
void testInvalidCardFlowFirstAttempt();
void testInvalidCardFlowSecondAttempt();
void testInvalidCardFlowThirdAttempt();
void testExponentialBackoff();
void testMaxInvalidAttempts();
void testWaitingSoundAfter10Seconds();
void testRelayTimingAccuracy();
void testSequentialRelayActivation();

#endif  // TEST_INTEGRATION_H
