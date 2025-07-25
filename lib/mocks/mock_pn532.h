#pragma once

#include <cstdint>

#define PN532_MIFARE_ISO14443A (0x00)

// Mock PN532 class
class Adafruit_PN532 {
private:
    uint8_t m_ss_pin;
    uint32_t m_firmwareVersion{0x0153};
    bool m_initialized{false};

    // Mock card data
    uint8_t* m_mockUID{nullptr};
    uint8_t m_mockUIDLength{0};
    bool m_hasCard{false};

public:
    // Constructor for hardware SPI (SS pin only)
    Adafruit_PN532(uint8_t ss) : m_ss_pin(ss) {}

    // Constructor for software SPI (CLK, MISO, MOSI, SS pins)
    Adafruit_PN532(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t ss)
        : m_ss_pin(ss), m_clk_pin(clk), m_miso_pin(miso), m_mosi_pin(mosi) {}

    bool begin() {
        m_initialized = true;
        return true;  // Mock successful initialization
    }

    uint32_t getFirmwareVersion() const {
        return m_initialized ? m_firmwareVersion : 0;
    }

    void SAMConfig() {
        // Mock implementation - does nothing
    }

    bool readPassiveTargetID(uint8_t /*cardbaudrate*/, uint8_t* uid, uint8_t* uidLength,
                             uint16_t /*timeout*/ = 0) {
        if (!m_initialized || !m_hasCard) {
            return false;
        }

        if ((m_mockUID != nullptr) && (uid != nullptr) && (uidLength != nullptr)) {
            // Simple copy instead of memcpy
            for (uint8_t i = 0; i < m_mockUIDLength; i++) {
                uid[i] = m_mockUID[i];
            }
            *uidLength = m_mockUIDLength;
            return true;
        }

        return false;
    }

    // Test helper methods
    void setMockCard(uint8_t* uid, uint8_t uidLength) {
        m_mockUID = uid;
        m_mockUIDLength = uidLength;
        m_hasCard = true;
    }

    void removeMockCard() {
        m_hasCard = false;
    }

    void setFirmwareVersion(uint32_t version) {
        m_firmwareVersion = version;
    }
    void reset() {
        // Mock reset functionality
        m_initialized = false;
        m_firmwareVersion = 0x0153;  // Reset to default mock version
        removeMockCard();
    }

private:
    uint8_t m_clk_pin{0};
    uint8_t m_miso_pin{0};
    uint8_t m_mosi_pin{0};
};
