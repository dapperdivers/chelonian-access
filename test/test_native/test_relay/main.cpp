#include <unity.h>
#include "test_helpers.h"

// External fixture from test_helpers.cpp
extern RelayTestFixture* relayFixture;

// External base functions from test_helpers.cpp
extern void baseSetUp();
extern void baseTearDown();

// Declare test functions
void testRelayInitialState();
void testSetSingleRelay();
void testSetAllRelays();
void testInvalidRelayNumber();
void testRelayStateTransitions();
void testRapidRelaySwitching();
void testSequentialRelayOperations();

// Relay-specific setUp and tearDown
void setUp() {
    baseSetUp();
    printf("Creating RelayTestFixture...\n");
    relayFixture = new RelayTestFixture();
    printf("RelayTestFixture created successfully\n");

    // Initialize the relay mock
    if (relayFixture && relayFixture->relays) {
        relayFixture->relays->begin();
    }
}

void tearDown() {
    // Delete the fixture before calling baseTearDown
    if (relayFixture) {
        delete relayFixture;
        relayFixture = nullptr;
    }
    baseTearDown();
}

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
