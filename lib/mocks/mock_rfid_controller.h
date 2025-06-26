#pragma once
#include <cstdint>
#include <cstring>
#include <vector>
#include "IRFIDController.h"
#include "mock_pn532.h"

class MockRFIDController : public IRFIDController {
public:
    MockRFIDController() : beginCalled(false), lastUIDLength(0), validateUIDResult(false) {}

    bool begin() override {
        beginCalled = true;
        return true;
    }
    bool readCard(uint8_t* uid, uint8_t* uidLength) override {
        if (mockUIDs.empty())
            return false;
        std::memcpy(uid, mockUIDs.back().data(), mockUIDs.back().size());
        *uidLength = mockUIDs.back().size();
        lastUIDLength = *uidLength;
        return true;
    }
    bool validateUID(const uint8_t* uid, uint8_t uidLength) override {
        lastValidatedUID.assign(uid, uid + uidLength);
        lastUIDLength = uidLength;
        return validateUIDResult;
    }
    void addUID4B(const uint8_t* uid) override {}
    void addUID7B(const uint8_t* uid) override {}
    uint32_t getFirmwareVersion() override {
        return 0;
    }
    void printFirmwareVersion() override {}
    void initializeDefaultUIDs() override {}

    void addMockUID(const std::vector<uint8_t>& uid) {
        mockUIDs.push_back(uid);
    }

    bool beginCalled;
    std::vector<std::vector<uint8_t>> mockUIDs;
    std::vector<uint8_t> lastValidatedUID;
    uint8_t lastUIDLength;
    bool validateUIDResult;
};
