#pragma once
#include <cstdint>

class IRFIDController {
public:
    virtual bool begin() = 0;
    virtual bool readCard(uint8_t* uid, uint8_t* uidLength) = 0;
    virtual bool validateUID(const uint8_t* uid, uint8_t uidLength) = 0;
    virtual void addUID4B(const uint8_t* uid) = 0;
    virtual void addUID7B(const uint8_t* uid) = 0;
    virtual uint32_t getFirmwareVersion() = 0;
    virtual void printFirmwareVersion() = 0;
    virtual void initializeDefaultUIDs() = 0;
    virtual ~IRFIDController() = default;
};
