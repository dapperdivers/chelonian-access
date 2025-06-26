#pragma once

#include <stdint.h>

#include "IAudioController.h"
#include "IRFIDController.h"
#include "IRelayController.h"

extern uint8_t invalidAttempts;
extern bool scanned;
extern bool impatient;
extern unsigned long relayActivatedTime;
extern bool relayActive;

constexpr uint8_t MAXIMUM_INVALID_ATTEMPTS = 13;
extern const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS];

constexpr uint32_t RELAY1_DURATION = 1000;
constexpr uint32_t RELAY2_DURATION = 1000;
constexpr uint8_t RELAY1_PIN = 9;
constexpr uint8_t RELAY2_PIN = 10;
constexpr uint8_t RELAY3_PIN = 11;
constexpr uint8_t RELAY4_PIN = 12;

// Relay state enum
enum RelayState { RELAY_IDLE, RELAY1_ACTIVE, RELAY2_PENDING, RELAY2_ACTIVE };
extern RelayState currentRelayState;

class AccessService {
public:
    AccessService(IRFIDController& rfid, IRelayController& relays, IAudioController& audio);
    void loop();
    void handleRelaySequence();
    void activateRelays();

private:
    IRFIDController& rfid_;
    IRelayController& relays_;
    IAudioController& audio_;
};
