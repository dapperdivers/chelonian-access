#pragma once

#include <cstdint>

class IRelayController {
public:
    virtual void begin() = 0;
    virtual void setRelay(uint8_t relay, bool state) = 0;
    virtual void setAllRelays(bool state) = 0;
    virtual bool getRelayState(uint8_t relay) const = 0;
    virtual ~IRelayController() = default;
};
