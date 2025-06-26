#include "test_helpers.h"
#include <unity.h>
#include "mock_arduino.h"

// Global variable to track total delay time
extern int mock_delay_time;

// Common test utilities and helper functions
void resetTestState() {
    resetMockState();
}

// Initialize fixture pointers
AudioTestFixture* audioFixture = nullptr;
RelayTestFixture* relayFixture = nullptr;
RFIDTestFixture* rfidFixture = nullptr;
AccessTestFixture* accessFixture = nullptr;

// Base setUp and tearDown functions that only reset mock state
void baseSetUp() {
    TEST_PROTECT();
    resetMockState();
}

void baseTearDown() {
    TEST_PROTECT();
    // Each test suite is responsible for cleaning up its own fixture
    // This function only resets the mock state
}

// Centralized mock initialization function
void initializeMocks(AudioTestFixture* audioFix, RelayTestFixture* relayFix,
                     RFIDTestFixture* rfidFix) {
    if (audioFix && audioFix->audio) {
        audioFix->audio->begin();
    }
    if (relayFix && relayFix->relays) {
        relayFix->relays->begin();
    }
    if (rfidFix && rfidFix->rfid) {
        rfidFix->rfid->begin();
    }
}

// Integration test helper functions
void advanceMockTime(millis_t ms) {
    mockMillis += ms;
}

millis_t getTotalDelayTime() {
    return mock_delay_time;
}

uint8_t getPinTransitionCount(uint8_t pin) {
    uint8_t count = 0;
    uint8_t historyCount = getPinHistoryCount();

    for (uint8_t i = 0; i < historyCount; i++) {
        PinStateChange change = getPinHistoryEntry(i);
        if (change.pin == pin) {
            count++;
        }
    }

    return count;
}

// Helper function to verify pin state sequence
bool verifyPinSequence(uint8_t pin, const uint8_t* expectedStates, uint8_t stateCount) {
    uint8_t pinChangeIndex = 0;
    uint8_t historyCount = getPinHistoryCount();

    for (uint8_t i = 0; i < historyCount; i++) {
        PinStateChange change = getPinHistoryEntry(i);
        if (change.pin == pin) {
            if (pinChangeIndex >= stateCount) {
                return false;  // Too many state changes
            }
            if (change.state != expectedStates[pinChangeIndex]) {
                return false;  // Wrong state
            }
            pinChangeIndex++;
        }
    }

    return pinChangeIndex == stateCount;  // Verify we saw all expected states
}
