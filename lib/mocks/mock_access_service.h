#pragma once
#include "access_service.h"
#include "mock_audio_controller.h"
#include "mock_relay_controller.h"
#include "mock_rfid_controller.h"

class MockAccessService : public AccessService {
public:
    MockAccessService(IRFIDController& rfid, IRelayController& relays, IAudioController& audio)
        : AccessService(rfid, relays, audio),
          mockRFID(dynamic_cast<MockRFIDController*>(&rfid)),
          mockRelays(dynamic_cast<MockRelayController*>(&relays)),
          mockAudio(dynamic_cast<MockAudioController*>(&audio)) {}

    // Pointers to the actual mocks for direct inspection in tests
    MockRFIDController* mockRFID;
    MockRelayController* mockRelays;
    MockAudioController* mockAudio;
};
