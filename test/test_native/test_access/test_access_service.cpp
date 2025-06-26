#include <unity.h>
#include "access_service.h"
#include "test_helpers.h"

// External fixtures from test_helpers.cpp
extern AccessTestFixture* accessFixture;

// External variables from AccessService
extern uint8_t invalidAttempts;
extern bool scanned;
extern bool impatient;
extern unsigned long relayActivatedTime;  // Fixed type to match access_service.cpp
extern bool relayActive;
extern enum RelayState currentRelayState;  // Added enum keyword
extern const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS];

// Basic Tests
void testAccessServiceInitialization() {
    // Verify all components were initialized
    TEST_ASSERT_TRUE(accessFixture->mockRFID->beginCalled);
    TEST_ASSERT_TRUE(accessFixture->mockAudio->initialized);
}

// Valid Card Access Tests
void testValidCardFlow() {
    // Set up a valid card
    std::vector<uint8_t> validCard = {0xB4, 0x12, 0x34, 0x56};
    accessFixture->mockRFID->addMockUID(validCard);
    accessFixture->mockRFID->validateUIDResult = true;

    // Clear relay history
    accessFixture->mockRelays->relayHistory.clear();

    // Process the card
    accessFixture->service->loop();

    // Verify acceptance sound was played
    TEST_ASSERT_EQUAL(IAudioController::SOUND_ACCEPTED,
                      accessFixture->mockAudio->getLastPlayedTrack());

    // Verify relay activation started
    TEST_ASSERT_TRUE(relayActive);
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);

    // Verify relay 1 was activated
    TEST_ASSERT_TRUE(accessFixture->mockRelays->setRelayCalled);
    TEST_ASSERT_EQUAL(RELAY1_PIN, accessFixture->mockRelays->lastPin);
    TEST_ASSERT_TRUE(accessFixture->mockRelays->lastState);

    // Advance time by relay 1 duration
    advanceMockTime(RELAY1_DURATION + 100);
    accessFixture->service->handleRelaySequence();

    // Verify relay sequence progression
    TEST_ASSERT_EQUAL(RELAY2_ACTIVE, currentRelayState);

    // Advance time by relay 2 duration
    advanceMockTime(RELAY2_DURATION + 100);
    accessFixture->service->handleRelaySequence();

    // Verify sequence completed
    TEST_ASSERT_FALSE(relayActive);
    TEST_ASSERT_EQUAL(RELAY_IDLE, currentRelayState);
}

// Invalid Card Access Tests
void testInvalidCardFirstAttempt() {
    resetTestState();

    // Set up an invalid card
    std::vector<uint8_t> invalidCard = {0xFF, 0xFF, 0xFF, 0xFF};
    accessFixture->mockRFID->addMockUID(invalidCard);
    accessFixture->mockRFID->validateUIDResult = false;

    // Clear relay history
    accessFixture->mockRelays->relayHistory.clear();

    // Process the invalid card
    accessFixture->service->loop();

    // Verify first denial sound was played
    TEST_ASSERT_EQUAL(IAudioController::SOUND_DENIED_1,
                      accessFixture->mockAudio->getLastPlayedTrack());

    // Verify no relay activation
    TEST_ASSERT_FALSE(relayActive);
    TEST_ASSERT_EQUAL(0, accessFixture->mockRelays->relayHistory.size());

    // Verify invalid attempts incremented
    TEST_ASSERT_EQUAL(1, invalidAttempts);
}

void testMultipleInvalidAttempts() {
    // Set up an invalid card
    std::vector<uint8_t> invalidCard = {0xFF, 0xFF, 0xFF, 0xFF};
    accessFixture->mockRFID->validateUIDResult = false;

    // First attempt
    accessFixture->mockRFID->addMockUID(invalidCard);
    accessFixture->service->loop();
    TEST_ASSERT_EQUAL(IAudioController::SOUND_DENIED_1,
                      accessFixture->mockAudio->getLastPlayedTrack());
    TEST_ASSERT_EQUAL(1, invalidAttempts);

    // Clear card and wait
    accessFixture->mockRFID->mockUIDs.clear();
    advanceMockTime(5000);

    // Second attempt
    accessFixture->mockRFID->addMockUID(invalidCard);
    accessFixture->service->loop();
    TEST_ASSERT_EQUAL(IAudioController::SOUND_DENIED_2,
                      accessFixture->mockAudio->getLastPlayedTrack());
    TEST_ASSERT_EQUAL(2, invalidAttempts);

    // Clear card and wait
    accessFixture->mockRFID->mockUIDs.clear();
    advanceMockTime(7000);

    // Third attempt
    accessFixture->mockRFID->addMockUID(invalidCard);
    accessFixture->service->loop();
    TEST_ASSERT_EQUAL(IAudioController::SOUND_DENIED_3,
                      accessFixture->mockAudio->getLastPlayedTrack());
    TEST_ASSERT_EQUAL(3, invalidAttempts);
}

// Timing Tests
void testWaitingSoundAfter10Seconds() {
    resetTestState();

    // No card present
    // Advance time to trigger waiting sound
    setMockMillis(11000);  // 11 seconds

    // Process loop
    accessFixture->service->loop();

    // Verify waiting sound was played
    TEST_ASSERT_EQUAL(IAudioController::SOUND_WAITING,
                      accessFixture->mockAudio->getLastPlayedTrack());

    // Verify impatient flag set
    TEST_ASSERT_TRUE(impatient);
}

void testRelaySequenceTiming() {
    // Set up a valid card
    std::vector<uint8_t> validCard = {0xB4, 0x12, 0x34, 0x56};
    accessFixture->mockRFID->addMockUID(validCard);
    accessFixture->mockRFID->validateUIDResult = true;

    // Clear relay history
    accessFixture->mockRelays->relayHistory.clear();

    // Process the card to start relay sequence
    millis_t startTime = millis();
    accessFixture->service->loop();

    // Verify relay activation started
    TEST_ASSERT_TRUE(relayActive);
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);

    // Advance time by relay 1 duration and handle sequence
    advanceMockTime(RELAY1_DURATION + 50);
    accessFixture->service->handleRelaySequence();

    // Verify switched to relay 2
    TEST_ASSERT_EQUAL(RELAY2_ACTIVE, currentRelayState);

    // Advance time by relay 2 duration
    advanceMockTime(RELAY2_DURATION + 50);
    accessFixture->service->handleRelaySequence();

    // Verify sequence completed
    TEST_ASSERT_FALSE(relayActive);
    TEST_ASSERT_EQUAL(RELAY_IDLE, currentRelayState);

    millis_t totalDuration = millis() - startTime;
    TEST_ASSERT_TRUE(totalDuration >= (RELAY1_DURATION + RELAY2_DURATION));
}

// Advanced Tests
void testSequentialRelayActivation() {
    // Set up a valid card
    std::vector<uint8_t> validCard = {0xB4, 0x12, 0x34, 0x56};
    accessFixture->mockRFID->addMockUID(validCard);
    accessFixture->mockRFID->validateUIDResult = true;

    // Clear relay history
    accessFixture->mockRelays->relayHistory.clear();

    // Process card - activates relay sequence
    accessFixture->service->loop();

    // Verify relay 1 is on
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);
    TEST_ASSERT_TRUE(accessFixture->mockRelays->setRelayCalled);
    TEST_ASSERT_EQUAL(RELAY1_PIN, accessFixture->mockRelays->lastPin);
    TEST_ASSERT_TRUE(accessFixture->mockRelays->lastState);

    size_t historyAfterRelay1 = accessFixture->mockRelays->relayHistory.size();

    // Advance time to switch to relay 2
    advanceMockTime(RELAY1_DURATION + 100);
    accessFixture->service->handleRelaySequence();

    // Verify switched to relay 2
    TEST_ASSERT_EQUAL(RELAY2_ACTIVE, currentRelayState);

    // Should have turned off relay 1 and turned on relay 2
    TEST_ASSERT_TRUE(accessFixture->mockRelays->relayHistory.size() > historyAfterRelay1);

    // Advance time to complete sequence
    advanceMockTime(RELAY2_DURATION + 100);
    accessFixture->service->handleRelaySequence();

    // Verify sequence completed
    TEST_ASSERT_EQUAL(RELAY_IDLE, currentRelayState);
    TEST_ASSERT_FALSE(relayActive);

    // Verify relay history shows complete sequence
    bool relay1On = false, relay1Off = false;
    bool relay2On = false, relay2Off = false;

    for (const auto& entry : accessFixture->mockRelays->relayHistory) {
        if (entry.first == RELAY1_PIN && entry.second)
            relay1On = true;
        if (entry.first == RELAY1_PIN && !entry.second)
            relay1Off = true;
        if (entry.first == RELAY2_PIN && entry.second)
            relay2On = true;
        if (entry.first == RELAY2_PIN && !entry.second)
            relay2Off = true;
    }

    TEST_ASSERT_TRUE(relay1On);
    TEST_ASSERT_TRUE(relay1Off);
    TEST_ASSERT_TRUE(relay2On);
    TEST_ASSERT_TRUE(relay2Off);
}

void testExponentialBackoff() {
    // Set up an invalid card
    std::vector<uint8_t> invalidCard = {0xFF, 0xFF, 0xFF, 0xFF};
    accessFixture->mockRFID->validateUIDResult = false;

    // Test that invalid attempts increase
    for (uint8_t i = 0; i < 5; i++) {
        accessFixture->mockRFID->addMockUID(invalidCard);
        accessFixture->service->loop();

        // Verify attempts incremented
        TEST_ASSERT_EQUAL(i + 1, invalidAttempts);

        // Clear card and wait
        accessFixture->mockRFID->mockUIDs.clear();
        advanceMockTime(10000);
    }
}

void testMaxInvalidAttempts() {
    // Set up an invalid card
    std::vector<uint8_t> invalidCard = {0xFF, 0xFF, 0xFF, 0xFF};
    accessFixture->mockRFID->validateUIDResult = false;

    // Make maximum invalid attempts
    for (uint8_t i = 0; i < MAXIMUM_INVALID_ATTEMPTS; i++) {
        accessFixture->mockRFID->addMockUID(invalidCard);
        accessFixture->service->loop();
        accessFixture->mockRFID->mockUIDs.clear();
        advanceMockTime(70000);  // Max delay time
    }

    // Verify attempts capped at maximum - 1
    TEST_ASSERT_EQUAL(MAXIMUM_INVALID_ATTEMPTS - 1, invalidAttempts);
}

// Direct Access Tests
void testDirectRelayActivation() {
    TEST_ASSERT_NOT_NULL(accessFixture);
    TEST_ASSERT_NOT_NULL(accessFixture->service);

    // Directly activate relays
    accessFixture->service->activateRelays();

    // Verify state
    TEST_ASSERT_TRUE(relayActive);
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);
    TEST_ASSERT_NOT_EQUAL(0, relayActivatedTime);

    // Verify relay 1 was turned on
    TEST_ASSERT_TRUE(accessFixture->mockRelays->setRelayCalled);
    TEST_ASSERT_EQUAL(RELAY1_PIN, accessFixture->mockRelays->lastPin);
    TEST_ASSERT_TRUE(accessFixture->mockRelays->lastState);
}

void testResetAfterValidCard() {
    // Set invalid attempts to simulate previous failures

    invalidAttempts = 5;

    // Set up a valid card
    std::vector<uint8_t> validCard = {0xB4, 0x12, 0x34, 0x56};
    accessFixture->mockRFID->addMockUID(validCard);
    accessFixture->mockRFID->validateUIDResult = true;

    // Process the valid card
    accessFixture->service->loop();

    // Verify acceptance
    TEST_ASSERT_EQUAL(IAudioController::SOUND_ACCEPTED,
                      accessFixture->mockAudio->getLastPlayedTrack());

    // Verify invalid attempts were reset
    TEST_ASSERT_EQUAL(0, invalidAttempts);
}

// MockAccessService-specific tests
void testMockAccessServiceCreation() {
    MockAccessService* service = new MockAccessService(
        *accessFixture->mockRFID, *accessFixture->mockRelays, *accessFixture->mockAudio);

    // Verify we can access the mocks through the service
    TEST_ASSERT_NOT_NULL(service->mockRFID);
    TEST_ASSERT_NOT_NULL(service->mockRelays);
    TEST_ASSERT_NOT_NULL(service->mockAudio);

    // Verify the mocks are the same instances
    TEST_ASSERT_EQUAL_PTR(accessFixture->mockRFID, service->mockRFID);
    TEST_ASSERT_EQUAL_PTR(accessFixture->mockRelays, service->mockRelays);
    TEST_ASSERT_EQUAL_PTR(accessFixture->mockAudio, service->mockAudio);

    delete service;
}
