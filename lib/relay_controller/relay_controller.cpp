#include "relay_controller.h"

#ifdef UNIT_TEST
#include "mock_esp_log.h"  // Add ESP_LOGX header
#else
#include "esp_log.h"
#endif

static const char* TAG = "RELAY";  // Add TAG definition
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

        ESP_LOGE(TAG, "Relay %u initialized on pin %u", i + 1, m_relays[i].pin);

        setRelay(i, false);  // Initialize all relays to OFF state
    }
}

void RelayController::setRelay(uint8_t relay, bool state) {
    if (isValidRelay(relay)) {
        m_relays[relay].state = state;
        // Active LOW relay logic
        digitalWrite(m_relays[relay].pin, static_cast<uint8_t>(!state));

        ESP_LOGE(TAG, "%lu ms - Relay %u set to %s", millis(), relay + 1, state ? "ON" : "OFF");
    }
}

void RelayController::setAllRelays(bool state) {
    ESP_LOGE(TAG, "%lu ms - All relays set to %s", millis(), state ? "ON" : "OFF");

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
