#pragma once

#include <array>
#include <cstdint>  // Include for uint8_t
#include "IRelayController.h"
#include "mock_arduino.h"  // Include for definitions like OUTPUT, HIGH, LOW

// Define function pointer types for hardware interaction
using pinMode_t = void (*)(uint8_t pin, uint8_t mode);
using digitalWrite_t = void (*)(uint8_t pin, uint8_t val);

class RelayController : public IRelayController {
public:
    // Constructor accepts function pointers for hardware interaction
    RelayController(pinMode_t pinMode_func, digitalWrite_t digitalWrite_func,
                    uint8_t relay1_pin = 9, uint8_t relay2_pin = 10, uint8_t relay3_pin = 20,
                    uint8_t relay4_pin = 21);

    void begin() override;
    void setRelay(uint8_t relay, bool state) override;
    void setAllRelays(bool state) override;
    bool getRelayState(uint8_t relay) const override;

private:
    struct RelayPin {
        uint8_t pin;
        bool state;
    };

    static constexpr uint8_t NUM_RELAYS = 4;
    std::array<RelayPin, NUM_RELAYS> m_relays{};

    // Function pointers for hardware interaction
    pinMode_t m_pinMode;
    digitalWrite_t m_digitalWrite;

    static bool isValidRelay(uint8_t relay);
};
