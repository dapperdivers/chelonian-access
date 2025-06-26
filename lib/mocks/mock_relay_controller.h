#pragma once
#include <vector>
#include "IRelayController.h"

class MockRelayController : public IRelayController {
public:
    MockRelayController() : lastPin(0), lastState(false), setRelayCalled(false) {}

    void setAllRelays(bool state) override {}
    void setRelay(uint8_t relay, bool state) override {
        lastPin = relay;
        lastState = state;
        setRelayCalled = true;
        relayHistory.push_back({relay, state});
    }
    bool getRelayState(uint8_t relay) const override {
        return false;
    }
    void begin() override {}

    // Optionally, add your mock tracking variables here
    uint8_t lastPin;
    bool lastState;
    bool setRelayCalled;
    std::vector<std::pair<uint8_t, bool>> relayHistory;
};
