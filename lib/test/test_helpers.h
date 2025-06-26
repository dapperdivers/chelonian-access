#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <unity.h>
#include "mock_access_service.h"
#include "mock_arduino.h"
#include "mock_audio_controller.h"
#include "mock_relay_controller.h"
#include "mock_rfid_controller.h"
#include "relay_controller.h"  // Include for RelayController definition

// Test fixtures
class AudioTestFixture {
public:
    IAudioController* audio;
    MockAudioController* mock;

    AudioTestFixture() : mock(new MockAudioController()), audio(mock) {}
    ~AudioTestFixture() {
        delete mock;
    }
};

class RFIDTestFixture {
public:
    IRFIDController* rfid;
    MockRFIDController* mock;

    RFIDTestFixture() : mock(new MockRFIDController()), rfid(mock) {}
    ~RFIDTestFixture() {
        delete mock;
    }
};

// Access test fixture
class AccessTestFixture {
public:
    MockRFIDController* mockRFID;
    MockRelayController* mockRelays;
    MockAudioController* mockAudio;
    AccessService* service;

    struct {
        uint8_t lastPlayedTrack = 0;
        bool relayActive = false;
        millis_t relayActivatedTime = 0;
        uint8_t invalidAttempts = 0;
    } state;

    // Constructor accepts injected mock controllers
    AccessTestFixture(MockRFIDController* rfid_mock, MockRelayController* relays_mock,
                      MockAudioController* audio_mock)
        : mockRFID(rfid_mock), mockRelays(relays_mock), mockAudio(audio_mock) {
        service = new AccessService(*mockRFID, *mockRelays, *mockAudio);
    }

    ~AccessTestFixture() {
        delete service;
        // Mocks are now owned by the calling code (main.cpp setUp)
        // delete mockAudio;
        // delete mockRelays;
        // delete mockRFID;
    }
};

class RelayTestFixture {
public:
    RelayController* relays;  // Use the concrete RelayController
    // We no longer need a separate mock pointer here as we are testing the real controller
    // MockRelayController* mock;

    RelayTestFixture() {
        // Instantiate the real RelayController, injecting mock Arduino functions
        relays = new RelayController(::pinMode, ::digitalWrite);
        // mock = static_cast<MockRelayController*>(relays); // This cast is no longer valid
    }
    ~RelayTestFixture() {
        delete relays;
    }
};

// Global fixture pointers
extern AudioTestFixture* audioFixture;
extern RelayTestFixture* relayFixture;
extern RFIDTestFixture* rfidFixture;
extern AccessTestFixture* accessFixture;

void setUp();
void tearDown();

// Helper functions
void resetTestState();
void advanceMockTime(millis_t ms);
millis_t getTotalDelayTime();
uint8_t getPinTransitionCount(uint8_t pin);
bool verifyPinSequence(uint8_t pin, const uint8_t* expectedStates, uint8_t stateCount);

// Centralized mock initialization
void initializeMocks(AudioTestFixture* audioFix, RelayTestFixture* relayFix,
                     RFIDTestFixture* rfidFix);

#endif  // TEST_HELPERS_H
