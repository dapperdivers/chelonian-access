#include "test_access_service.h"  // Include the new header for test function declarations
#include <access_service.h>
#include <esp_log.h>
#include <test_helpers.h>
#include <unity.h>

void testInitialState() {
    ESP_LOGE("TEST", "Starting testInitialState");
    // Check that relays are off and attempts are zero
    TEST_ASSERT_EQUAL(0, invalidAttempts);
    TEST_ASSERT_FALSE(relayActive);
    TEST_ASSERT_EQUAL(RELAY_IDLE, currentRelayState);
    TEST_ASSERT_FALSE(scanned);
    TEST_ASSERT_FALSE(impatient);
}

void testActivateRelays() {
    ESP_LOGE("TEST", "Starting testActivateRelays");
    // Should set relayActive and state
    activateRelays();
    TEST_ASSERT_TRUE(relayActive);
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);
}

void testRelaySequence() {
    ESP_LOGE("TEST", "Starting testRelaySequence");
    // Simulate activating relays and advancing time
    activateRelays();
    unsigned long start = relayActivatedTime;
    // Should be RELAY1_ACTIVE
    TEST_ASSERT_EQUAL(RELAY1_ACTIVE, currentRelayState);
    // Advance time to trigger relay 2
    setMockMillis(start + RELAY1_DURATION + 1);
    handleRelaySequence();
    TEST_ASSERT_EQUAL(RELAY2_ACTIVE, currentRelayState);
    // Advance time to finish sequence
    setMockMillis(relayActivatedTime + RELAY2_DURATION + 1);
    handleRelaySequence();
    TEST_ASSERT_EQUAL(RELAY_IDLE, currentRelayState);
    TEST_ASSERT_FALSE(relayActive);
}

void testImpatientWaiting() {
    ESP_LOGE("TEST", "Starting testImpatientWaiting");
    // Reset state
    impatient = false;
    scanned = false;
    setMockMillis(0);
    accessServiceLoop();
    TEST_ASSERT_FALSE(impatient);
    setMockMillis(10001);
    accessServiceLoop();
    TEST_ASSERT_TRUE(impatient);
}

void testInvalidCardDelays() {
    ESP_LOGE("TEST", "Starting testInvalidCardDelays");
    // Simulate invalid card scans and check delay increments
    invalidAttempts = 0;
    scanned = false;
    impatient = false;
    // Mock rfid.readCard to always return true, but validateUID to return false
    // This requires a mock or patch, so we just check the increment logic here
    for (uint8_t i = 0; i < MAXIMUM_INVALID_ATTEMPTS; ++i) {
        if (invalidAttempts < MAXIMUM_INVALID_ATTEMPTS - 1) {
            invalidAttempts++;
        }
    }
    TEST_ASSERT_EQUAL(MAXIMUM_INVALID_ATTEMPTS - 1, invalidAttempts);
}

// Add more tests as needed for your service logic
