#include <unity.h>
#include "access_service.h"
#include "test_helpers.h"

// External fixture from test_helpers.cpp
extern AccessTestFixture* accessFixture;

// External base functions from test_helpers.cpp
extern void baseSetUp();
extern void baseTearDown();

// External variables from AccessService
extern uint8_t invalidAttempts;
extern bool scanned;
extern bool impatient;
extern unsigned long relayActivatedTime;
extern bool relayActive;
extern enum RelayState currentRelayState;

// Helper to reset AccessService state
void resetAccessServiceState() {
    invalidAttempts = 0;
    scanned = false;
    impatient = false;
    relayActivatedTime = 0;
    relayActive = false;
    currentRelayState = RELAY_IDLE;
}

// Declare test functions
void testAccessServiceInitialization();
void testValidCardFlow();
void testInvalidCardFirstAttempt();
void testMultipleInvalidAttempts();
void testRelaySequenceTiming();
void testSequentialRelayActivation();
void testExponentialBackoff();
void testMaxInvalidAttempts();
void testDirectRelayActivation();
void testResetAfterValidCard();
void testMockAccessServiceCreation();

// Access-specific setUp and tearDown
void setUp() {
    baseSetUp();
    printf("Creating AccessTestFixture...\n");

    // Create mock controllers
    MockRFIDController* mockRFID = new MockRFIDController();
    MockRelayController* mockRelays = new MockRelayController();
    MockAudioController* mockAudio = new MockAudioController();

    // Create AccessTestFixture with injected mocks
    accessFixture = new AccessTestFixture(mockRFID, mockRelays, mockAudio);

    printf("AccessTestFixture created successfully\n");

    // Initialize the mocks (now done in AccessTestFixture constructor)
    // if (accessFixture) {
    //     if (accessFixture->mockRFID) accessFixture->mockRFID->begin();
    //     if (accessFixture->mockAudio) accessFixture->mockAudio->begin();
    //     if (accessFixture->mockRelays) accessFixture->mockRelays->begin();
    // }

    // Reset AccessService state for each test
    resetAccessServiceState();
}

void tearDown() {
    // Delete the mock controllers and the fixture
    if (accessFixture) {
        // Mocks are now owned by setUp, delete them here
        delete accessFixture->mockRFID;
        delete accessFixture->mockRelays;
        delete accessFixture->mockAudio;

        delete accessFixture;
        accessFixture = nullptr;
    }
    baseTearDown();
}

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testAccessServiceInitialization);
    RUN_TEST(testValidCardFlow);
    RUN_TEST(testInvalidCardFirstAttempt);
    RUN_TEST(testMultipleInvalidAttempts);
    RUN_TEST(testRelaySequenceTiming);
    RUN_TEST(testSequentialRelayActivation);
    RUN_TEST(testExponentialBackoff);
    RUN_TEST(testMaxInvalidAttempts);
    RUN_TEST(testDirectRelayActivation);
    RUN_TEST(testResetAfterValidCard);
    RUN_TEST(testMockAccessServiceCreation);

    return UNITY_END();
}
