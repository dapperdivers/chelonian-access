#include "rfid_controller.h"
#include <array>
#include <cstring>

#ifdef UNIT_TEST
#include "mock_arduino.h"
#include "mock_esp_log.h"  // Add ESP_LOGX header
#include "mock_pn532.h"
#else
#include <Adafruit_PN532.h>
#include <Arduino.h>
#include "esp_log.h"
#endif

static const char* TAG = "RFID";  // Add TAG definition

RFIDController::RFIDController(uint8_t ss_pin)
    : m_ss_pin(ss_pin), m_nfc(new Adafruit_PN532(m_ss_pin)) {
    // Using SPI interface with Adafruit_PN532
}

RFIDController::~RFIDController() {
    delete m_nfc;
}

bool RFIDController::begin() {
    m_nfc->begin();  // Call begin, which returns void
    ESP_LOGE(TAG, "Initializing PN532...");

    delay(1000);  // Wait for the PN532 to initialize
    uint32_t versiondata = m_nfc->getFirmwareVersion();
    if (versiondata == 0) {
        ESP_LOGE(TAG, "Failed to initialize PN532!");
        return false;
    }
    ESP_LOGE(TAG, "PN532 initialized successfully!");

    // Configure board to read RFID tags
    m_nfc->SAMConfig();
    return true;
}

bool RFIDController::readCard(uint8_t* uid, uint8_t* uidLength) {
    bool result = m_nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);
    if (result) {
        ESP_LOGE(TAG, "%lu ms - Card detected: UID=", millis());
        char uidStr[50] = "";
        for (uint8_t i = 0; i < *uidLength; i++) {
            char hexBuf[5];
            sprintf(hexBuf, "%02X:", uid[i]);
            strcat(uidStr, hexBuf);
        }
        uidStr[strlen(uidStr) - 1] = '\0';  // Remove last colon
        ESP_LOGE(TAG, "%s", uidStr);
    } else {
        ESP_LOGE(TAG, "No card detected");
    }
    return result;
}

bool RFIDController::validateUID(const uint8_t* uid, uint8_t uidLength) {
    ESP_LOGE(TAG, "%lu ms - Validating UID=", millis());
    char uidStrValidate[50] = "";
    for (uint8_t i = 0; i < uidLength; i++) {
        char hexBuf[5];
        sprintf(hexBuf, "%02X:", uid[i]);
        strcat(uidStrValidate, hexBuf);
    }
    uidStrValidate[strlen(uidStrValidate) - 1] = '\0';  // Remove last colon
    ESP_LOGE(TAG, "%s", uidStrValidate);

    if (uidLength == 4) {
        for (uint8_t i = 0; i < m_num4BUIDs; i++) {
            if (compare4BUID(m_uids4B[i].data(), uid)) {
                ESP_LOGE(TAG, " - Authentication successful (4B)");
                return true;
            }
        }
    } else if (uidLength == 7) {
        for (uint8_t i = 0; i < m_num7BUIDs; i++) {
            if (compare7BUID(m_uids7B[i].data(), uid)) {
                ESP_LOGE(TAG, " - Authentication successful (7B)");
                return true;
            }
        }
    } else {
        ESP_LOGW(TAG, " - Authentication failed: Invalid UID length");
        return false;
    }

    ESP_LOGW(TAG, " - Authentication failed: No matching UID");
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
        ESP_LOGE(TAG, "Found chip PN5");
        ESP_LOGE(TAG, "%02X", (versiondata >> 24) & 0xFF);  // Log the value
        ESP_LOGE(TAG, "Firmware ver. %u.%u", (versiondata >> 16) & 0xFF, (versiondata >> 8) & 0xFF);
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
