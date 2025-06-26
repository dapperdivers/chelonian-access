#include <unity.h>
#include "test_helpers.h"

// External fixture from test_helpers.cpp
extern RFIDTestFixture* rfidFixture;

// External base functions from test_helpers.cpp
extern void baseSetUp();
extern void baseTearDown();

// Declare test functions
void testRfidInitialization();
void testRfidReadCard();
void testRfidValidateUID();
void testRfidMultipleCards();
void testRfidAddUIDs();
void testRfidInitializeDefaultUIDs();
void testRfidInvalidUidLength();
void testRfidPrintFirmwareVersion();

// RFID-specific setUp and tearDown
void setUp() {
    baseSetUp();
    printf("Creating RFIDTestFixture...\n");
    rfidFixture = new RFIDTestFixture();
    printf("RFIDTestFixture created successfully\n");

    // Initialize the RFID mock
    if (rfidFixture && rfidFixture->rfid) {
        rfidFixture->rfid->begin();
    }
}

void tearDown() {
    // Delete the fixture before calling baseTearDown
    if (rfidFixture) {
        delete rfidFixture;
        rfidFixture = nullptr;
    }

    baseTearDown();
}

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testRfidInitialization);
    RUN_TEST(testRfidReadCard);
    RUN_TEST(testRfidValidateUID);
    RUN_TEST(testRfidMultipleCards);
    RUN_TEST(testRfidAddUIDs);
    RUN_TEST(testRfidInitializeDefaultUIDs);
    RUN_TEST(testRfidInvalidUidLength);
    RUN_TEST(testRfidPrintFirmwareVersion);

    return UNITY_END();
}
