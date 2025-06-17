#include "rfid_controller.h"
#include <array>
#include <cstring>

RFIDController::RFIDController(uint8_t ss_pin)
    : m_ss_pin(ss_pin), m_nfc(new AdafruitPN532(m_ss_pin)) {
    // Using SPI interface with Adafruit_PN532
}

bool RFIDController::begin() {
    m_nfc->begin();

    uint32_t versiondata = m_nfc->getFirmwareVersion();
    if (versiondata == 0u) {
        HardwareSerial::print(F("Didn't find PN53x board"));
        return false;
    }

    // Configure board to read RFID tags
    m_nfc->samConfig();
    return true;
}

bool RFIDController::readCard(uint8_t* uid, uint8_t* uidLength) {
    return m_nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);
}

bool RFIDController::validateUID(const uint8_t* uid, uint8_t uidLength) {
    if (uidLength == 4) {
        for (uint8_t i = 0; i < m_num4BUIDs; i++) {
            if (compare4BUID(m_uids4B[i].data(), uid)) {
                return true;
            }
        }
    } else if (uidLength == 7) {
        for (uint8_t i = 0; i < m_num7BUIDs; i++) {
            if (compare7BUID(m_uids7B[i].data(), uid)) {
                return true;
            }
        }
    }
    return false;
}

void RFIDController::addUID4B(const uint8_t* uid) {
    if (m_num4BUIDs < MAX_4B_UIDS) {
        memcpy(m_uids4B[m_num4BUIDs].data(), uid, 4);
        m_num4BUIDs++;
    }
}

void RFIDController::addUID7B(const uint8_t* uid) {
    if (m_num7BUIDs < MAX_7B_UIDS) {
        memcpy(m_uids7B[m_num7BUIDs].data(), uid, 7);
        m_num7BUIDs++;
    }
}

uint32_t RFIDController::getFirmwareVersion() {
    return m_nfc->getFirmwareVersion();
}

void RFIDController::printFirmwareVersion() {
    uint32_t versiondata = getFirmwareVersion();
    if (versiondata != 0u) {
        HardwareSerial::print(F("Found chip PN5"));
        HardwareSerial::println((versiondata >> 24) & 0xFF, HEX);
        HardwareSerial::print(F("Firmware ver. "));
        HardwareSerial::print((versiondata >> 16) & 0xFF, DEC);
        HardwareSerial::print('.');
        HardwareSerial::println((versiondata >> 8) & 0xFF, DEC);
    }
}

void RFIDController::initializeDefaultUIDs() {
    // Initialize with test UIDs as specified
    std::array<uint8_t, 4> testUID4B = {0xB4, 0x12, 0x34, 0x56};
    std::array<uint8_t, 7> testUiD7B1 = {0x04, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    std::array<uint8_t, 7> testUiD7B2 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD};

    addUID4B(testUID4B.data());
    addUID7B(testUiD7B1.data());
    addUID7B(testUiD7B2.data());
}

bool RFIDController::compare4BUID(const uint8_t* uid1, const uint8_t* uid2) {
    return memcmp(uid1, uid2, 4) == 0;
}

bool RFIDController::compare7BUID(const uint8_t* uid1, const uint8_t* uid2) {
    return memcmp(uid1, uid2, 7) == 0;
}
