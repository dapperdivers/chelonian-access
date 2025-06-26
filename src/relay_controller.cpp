#include "relay_controller.h"
#include "IRelayController.h"

// Constructor implementation
RelayController::RelayController(pinMode_t pinMode_func, digitalWrite_t digitalWrite_func,
                                 uint8_t relay1_pin, uint8_t relay2_pin, uint8_t relay3_pin,
                                 uint8_t relay4_pin)
    : m_pinMode(pinMode_func), m_digitalWrite(digitalWrite_func) {
    m_relays[0] = {relay1_pin, false};
    m_relays[1] = {relay2_pin, false};
    m_relays[2] = {relay3_pin, false};
    m_relays[3] = {relay4_pin, false};
}

void RelayController::begin() {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        m_pinMode(m_relays[i].pin, OUTPUT);
        setRelay(i, false);  // Initialize all relays to OFF state
    }
}

void RelayController::setRelay(uint8_t relay, bool state) {
    if (isValidRelay(relay)) {
        m_relays[relay].state = state;
        // Active LOW relay logic
        m_digitalWrite(m_relays[relay].pin, static_cast<uint8_t>(!state));
    }
}

void RelayController::setAllRelays(bool state) {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        setRelay(i, state);
    }
}

bool RelayController::getRelayState(uint8_t relay) const {
    if (isValidRelay(relay)) {
        return m_relays[relay].state;
    }
    return false;
}

bool RelayController::isValidRelay(uint8_t relay) {
    return relay < NUM_RELAYS;
}
