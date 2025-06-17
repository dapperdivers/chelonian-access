#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void testRelayInitialState();
void testSetSingleRelay();
void testSetAllRelays();
void testInvalidRelayNumber();
void testRelayStateTransitions();
void testRapidRelaySwitching();
void testSequentialRelayOperations();

// setUp and tearDown are defined in test_helpers.cpp

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testRelayInitialState);
    RUN_TEST(testSetSingleRelay);
    RUN_TEST(testSetAllRelays);
    RUN_TEST(testInvalidRelayNumber);
    RUN_TEST(testRelayStateTransitions);
    RUN_TEST(testRapidRelaySwitching);
    RUN_TEST(testSequentialRelayOperations);

    return UNITY_END();
}
