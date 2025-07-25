#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <Arduino.h>
#include <unity.h>
#include "audio_controller.h"
#include "relay_controller.h"
#include "rfid_controller.h"

// Test fixtures
class AudioTestFixture {
public:
    AudioContoller* audio;

    AudioTestFixture() : audio(new AudioContoller(8, 7)) {}

    ~AudioTestFixture() {
        delete audio;
    }
};

class RelayTestFixture {
public:
    RelayController* relays;

    // Define the specific pins used for testing
    static constexpr uint8_t RELAY_PIN_1 = 9;
    static constexpr uint8_t RELAY_PIN_2 = 10;
    static constexpr uint8_t RELAY_PIN_3 = 20;
    static constexpr uint8_t RELAY_PIN_4 = 21;

    RelayTestFixture()
        : relays(new RelayController(RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4)) {
        relays->begin();
    }

    ~RelayTestFixture() {
        relays->setAllRelays(false);
        delete relays;
    }
};

class IntegrationTestFixture {
public:
    struct SystemState {
        bool relayActive;
        uint8_t invalidAttempts;
        uint8_t lastPlayedTrack;
        millis_t relayActivatedTime;

        void reset() {
            relayActive = false;
            invalidAttempts = 0;
            lastPlayedTrack = 0;
            relayActivatedTime = 0;
        }
    } state{};

    IntegrationTestFixture() {
        state.reset();
    }

    ~IntegrationTestFixture() {
        state.reset();
    }
};

class RFIDTestFixture {
public:
    RFIDController* rfid;

    RFIDTestFixture() : rfid(new RFIDController(10)) {
        // Don't reset mock state here - it's already done in setUp()
    }

    ~RFIDTestFixture() {
        delete rfid;
    }
};

// Global fixture pointers
extern AudioTestFixture* audioFixture;
extern RelayTestFixture* relayFixture;
extern IntegrationTestFixture* integrationFixture;
extern RFIDTestFixture* rfidFixture;

void setUp();
void tearDown();

// Integration test helper functions
void advanceMockTime(millis_t ms);
millis_t getTotalDelayTime();
uint8_t getPinTransitionCount(uint8_t pin);
void resetMockState();
bool verifyPinSequence(uint8_t pin, const uint8_t* expectedStates, uint8_t stateCount);

#endif  // TEST_HELPERS_H
