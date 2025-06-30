#pragma once

#include <Arduino.h>
#include <array>

class RelayController {
public:
    // ESP32-C3 GPIO pins
    RelayController(uint8_t relay1_pin = 0, uint8_t relay2_pin = 1, uint8_t relay3_pin = 2,
                    uint8_t relay4_pin = 3);
    void begin();
    void setRelay(uint8_t relay, bool state);
    void setAllRelays(bool state);
    bool getRelayState(uint8_t relay) const;

private:
    struct RelayPin {
        uint8_t pin;
        bool state;
    };

    static constexpr uint8_t NUM_RELAYS = 4;
    std::array<RelayPin, NUM_RELAYS> m_relays{};

    static bool isValidRelay(uint8_t relay);
};
