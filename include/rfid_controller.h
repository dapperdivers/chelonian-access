#pragma once

#include <array>

#ifdef UNIT_TEST
    #include "mock_pn532.h"  // Mock PN532 for unit testing
#else
    #include <Arduino.h>
    #include <Adafruit_PN532.h>
#endif

class RFIDController {
public:
    // ESP32-C3 SPI SS pin
    RFIDController(uint8_t ss_pin = 5);
    bool begin();
    bool readCard(uint8_t* uid, uint8_t* uidLength);
    bool validateUID(const uint8_t* uid, uint8_t uidLength);
    void addUID4B(const uint8_t* uid);
    void addUID7B(const uint8_t* uid);
    uint32_t getFirmwareVersion();
    void printFirmwareVersion();
    void initializeDefaultUIDs();

private:
    static constexpr uint8_t MAX_4B_UIDS = 1;
    static constexpr uint8_t MAX_7B_UIDS = 2;
    uint8_t m_ss_pin;
    AdafruitPN532* m_nfc;  // Using SPI interface
    
    std::array<std::array<uint8_t, 4>, MAX_4B_UIDS> m_uids4B{};
    std::array<std::array<uint8_t, 7>, MAX_7B_UIDS> m_uids7B{};
    uint8_t m_num4BUIDs{0};
    uint8_t m_num7BUIDs{0};

    static bool compare4BUID(const uint8_t* uid1, const uint8_t* uid2);
    static bool compare7BUID(const uint8_t* uid1, const uint8_t* uid2);
};