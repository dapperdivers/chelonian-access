#include <unity.h>
#include "mock_arduino.h"
#include "mock_pn532.h"
#include "rfid_controller.h"
#include "test_helpers.h"

// Test fixture

void testRfidInitialization() {
    // Test successful initialization
    TEST_ASSERT_TRUE(rfidFixture->rfid->begin());

    // Test firmware version retrieval
    uint32_t version = rfidFixture->rfid->getFirmwareVersion();
    TEST_ASSERT_NOT_EQUAL(0, version);
}

void testRfidDefaultUids() {
    rfidFixture->rfid->begin();
    rfidFixture->rfid->initializeDefaultUIDs();

    // Test 4-byte UID validation
    uint8_t testUID4B[] = {0xB4, 0x12, 0x34, 0x56};
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(testUID4B, 4));

    // Test 7-byte UID validation
    uint8_t testUiD7B1[] = {0x04, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(testUiD7B1, 7));

    uint8_t testUiD7B2[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD};
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(testUiD7B2, 7));

    // Test invalid UID
    uint8_t invalidUID[] = {0xFF, 0xFF, 0xFF, 0xFF};
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(invalidUID, 4));
}

void testRfidAddUids() {
    rfidFixture->rfid->begin();

    // Add a 4-byte UID
    uint8_t newUID4B[] = {0xAA, 0xBB, 0xCC, 0xDD};
    rfidFixture->rfid->addUID4B(newUID4B);
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(newUID4B, 4));

    // Add a 7-byte UID
    uint8_t newUID7B[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    rfidFixture->rfid->addUID7B(newUID7B);
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(newUID7B, 7));
}

void testRfidInvalidUidLength() {
    rfidFixture->rfid->begin();

    // Test UID with invalid length (not 4 or 7)
    uint8_t invalidLengthUID[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(invalidLengthUID, 5));

    uint8_t tooShortUID[] = {0x01, 0x02};
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(tooShortUID, 2));

    uint8_t tooLongUID[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(tooLongUID, 8));
}

void testRfidMaxUidLimit() {
    rfidFixture->rfid->begin();

    // Try to add more than MAX_4B_UIDS (1)
    uint8_t uid1[] = {0x11, 0x11, 0x11, 0x11};
    uint8_t uid2[] = {0x22, 0x22, 0x22, 0x22};

    rfidFixture->rfid->addUID4B(uid1);
    rfidFixture->rfid->addUID4B(uid2);  // This should be ignored

    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid1, 4));
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(uid2, 4));

    // Try to add more than MAX_7B_UIDS (2)
    uint8_t uid71[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    uint8_t uid72[] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
    uint8_t uid73[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};

    rfidFixture->rfid->addUID7B(uid71);
    rfidFixture->rfid->addUID7B(uid72);
    rfidFixture->rfid->addUID7B(uid73);  // This should be ignored

    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid71, 7));
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid72, 7));
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(uid73, 7));
}
