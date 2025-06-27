#include "relay_controller.h"

RelayController::RelayController(uint8_t relay1_pin, uint8_t relay2_pin, uint8_t relay3_pin,
                                 uint8_t relay4_pin) {
    m_relays[0] = {relay1_pin, false};
    m_relays[1] = {relay2_pin, false};
    m_relays[2] = {relay3_pin, false};
    m_relays[3] = {relay4_pin, false};
}

void RelayController::begin() {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        pinMode(m_relays[i].pin, OUTPUT);

        Serial.print(F("[RELAY] Relay "));
        Serial.print(i + 1);
        Serial.print(F(" initialized on pin "));
        Serial.println(m_relays[i].pin);

        setRelay(i, false);  // Initialize all relays to OFF state
    }
}

void RelayController::setRelay(uint8_t relay, bool state) {
    if (isValidRelay(relay)) {
        m_relays[relay].state = state;
        // Active LOW relay logic
        digitalWrite(m_relays[relay].pin, static_cast<uint8_t>(!state));

        Serial.print(F("[RELAY] "));
        Serial.print(millis());
        Serial.print(F("ms - Relay "));
        Serial.print(relay + 1);
        Serial.print(F(" set to "));
        Serial.println(state ? F("ON") : F("OFF"));
    }
}

void RelayController::setAllRelays(bool state) {
    Serial.print(F("[RELAY] "));
    Serial.print(millis());
    Serial.print(F("ms - All relays set to "));
    Serial.println(state ? F("ON") : F("OFF"));

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
