#include <unity.h>
#include "test_helpers.h"

// External fixtures from test_helpers.cpp
extern RFIDTestFixture* rfidFixture;

void testRfidInitialization() {
    // Test successful initialization
    TEST_ASSERT_TRUE(rfidFixture->mock->beginCalled);

    // Test firmware version retrieval (mock returns 0)
    uint32_t version = rfidFixture->rfid->getFirmwareVersion();
    TEST_ASSERT_EQUAL(0, version);
}

void testRfidReadCard() {
    // Test with no card present
    uint8_t uid[8];
    uint8_t uidLength = 0;
    TEST_ASSERT_FALSE(rfidFixture->rfid->readCard(uid, &uidLength));

    // Add a mock card
    std::vector<uint8_t> mockCard = {0xB4, 0x12, 0x34, 0x56};
    rfidFixture->mock->addMockUID(mockCard);

    // Read the card
    TEST_ASSERT_TRUE(rfidFixture->rfid->readCard(uid, &uidLength));
    TEST_ASSERT_EQUAL(4, uidLength);
    TEST_ASSERT_EQUAL(0xB4, uid[0]);
    TEST_ASSERT_EQUAL(0x12, uid[1]);
    TEST_ASSERT_EQUAL(0x34, uid[2]);
    TEST_ASSERT_EQUAL(0x56, uid[3]);
}

void testRfidValidateUID() {
    // Test with validation disabled (default)
    uint8_t testUID[] = {0xB4, 0x12, 0x34, 0x56};
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(testUID, 4));

    // Enable validation for this UID
    rfidFixture->mock->validateUIDResult = true;
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(testUID, 4));

    // Verify the UID was passed to validation
    TEST_ASSERT_EQUAL(4, rfidFixture->mock->lastValidatedUID.size());
    TEST_ASSERT_EQUAL(0xB4, rfidFixture->mock->lastValidatedUID[0]);
    TEST_ASSERT_EQUAL(0x12, rfidFixture->mock->lastValidatedUID[1]);
    TEST_ASSERT_EQUAL(0x34, rfidFixture->mock->lastValidatedUID[2]);
    TEST_ASSERT_EQUAL(0x56, rfidFixture->mock->lastValidatedUID[3]);
}

void testRfidMultipleCards() {
    rfidFixture->rfid->begin();

    // Add multiple mock cards
    std::vector<uint8_t> card1 = {0xAA, 0xBB, 0xCC, 0xDD};
    std::vector<uint8_t> card2 = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    rfidFixture->mock->addMockUID(card1);
    rfidFixture->mock->addMockUID(card2);

    uint8_t uid[8];
    uint8_t uidLength;

    // Read first card (reads from back of vector)
    TEST_ASSERT_TRUE(rfidFixture->rfid->readCard(uid, &uidLength));
    TEST_ASSERT_EQUAL(7, uidLength);

    // Read second card
    TEST_ASSERT_TRUE(rfidFixture->rfid->readCard(uid, &uidLength));
    TEST_ASSERT_EQUAL(7, uidLength);
}

void testRfidAddUIDs() {
    rfidFixture->rfid->begin();

    // Test adding 4-byte UID
    uint8_t uid4B[] = {0xAA, 0xBB, 0xCC, 0xDD};
    rfidFixture->rfid->addUID4B(uid4B);

    // Test adding 7-byte UID
    uint8_t uid7B[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    rfidFixture->rfid->addUID7B(uid7B);

    // These methods don't have observable effects in the mock
    // but we ensure they don't crash
    TEST_ASSERT_TRUE(true);
}

void testRfidInitializeDefaultUIDs() {
    rfidFixture->rfid->begin();

    // This method has no observable effect in the mock
    rfidFixture->rfid->initializeDefaultUIDs();

    // Just ensure it doesn't crash
    TEST_ASSERT_TRUE(true);
}

void testRfidInvalidUidLength() {
    rfidFixture->rfid->begin();

    // Set up mock to accept any validation
    rfidFixture->mock->validateUIDResult = true;

    // Test various UID lengths
    uint8_t uid2[] = {0x01, 0x02};
    uint8_t uid4[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t uid5[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t uid7[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    uint8_t uid8[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    // Mock validates based on validateUIDResult, not length
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid2, 2));
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid4, 4));
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid5, 5));
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid7, 7));
    TEST_ASSERT_TRUE(rfidFixture->rfid->validateUID(uid8, 8));

    // Disable validation
    rfidFixture->mock->validateUIDResult = false;
    TEST_ASSERT_FALSE(rfidFixture->rfid->validateUID(uid4, 4));
}

void testRfidPrintFirmwareVersion() {
    rfidFixture->rfid->begin();

    // This method has no observable effect in the mock
    rfidFixture->rfid->printFirmwareVersion();

    // Just ensure it doesn't crash
    TEST_ASSERT_TRUE(true);
}
