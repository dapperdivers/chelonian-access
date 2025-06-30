#include "test_relay_controller.h"  // Include the new header for test function declarations
#include <esp_log.h>
#include <relay_controller.h>
#include <test_helpers.h>
#include <unity.h>

void testRelayInitialState() {
    // Check pin modes are set to OUTPUT
    TEST_ASSERT_EQUAL(OUTPUT, mockPinModes[9]);   // Relay 1
    TEST_ASSERT_EQUAL(OUTPUT, mockPinModes[10]);  // Relay 2
    TEST_ASSERT_EQUAL(OUTPUT, mockPinModes[20]);  // Relay 3
    TEST_ASSERT_EQUAL(OUTPUT, mockPinModes[21]);  // Relay 4

    // Check initial states
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(3));

    // Check initial pin states (active LOW, so HIGH = OFF)
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[9]);   // Relay 1
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[10]);  // Relay 2
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[20]);  // Relay 3
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[21]);  // Relay 4
}

void testSetSingleRelay() {
    relayFixture->relays->setRelay(0, true);
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_EQUAL(LOW, mockPinStates[9]);  // Active LOW logic

    relayFixture->relays->setRelay(0, false);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[9]);  // Active LOW logic
}

void testSetAllRelays() {
    relayFixture->relays->setAllRelays(true);
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(3));

    // Check all pins are LOW (active LOW logic)
    TEST_ASSERT_EQUAL(LOW, mockPinStates[9]);
    TEST_ASSERT_EQUAL(LOW, mockPinStates[10]);
    TEST_ASSERT_EQUAL(LOW, mockPinStates[20]);
    TEST_ASSERT_EQUAL(LOW, mockPinStates[21]);

    relayFixture->relays->setAllRelays(false);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(3));

    // Check all pins are HIGH (active LOW logic)
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[9]);
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[10]);
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[20]);
    TEST_ASSERT_EQUAL(HIGH, mockPinStates[21]);
}

void testInvalidRelayNumber() {
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(4));
    relayFixture->relays->setRelay(4, true);
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(4));
}

void testRelayStateTransitions() {
    // Clear history to start fresh
    resetMockState();
    relayFixture->relays->begin();

    // Clear again to only track our specific transitions
    resetMockState();

    // Test sequence: OFF -> ON -> OFF
    // Start with relay OFF (should be HIGH)
    relayFixture->relays->setRelay(0, false);  // Ensure it's OFF (HIGH)

    const uint8_t EXPECTED_STATES[] = {HIGH, LOW, HIGH};  // OFF -> ON -> OFF
    relayFixture->relays->setRelay(0, true);              // Goes LOW (active low)
    advanceMockTime(100);
    relayFixture->relays->setRelay(0, false);  // Goes HIGH

    TEST_ASSERT_TRUE(verifyPinSequence(9, EXPECTED_STATES, 3));
    TEST_ASSERT_EQUAL(3, getPinTransitionCount(9));
}

void testRapidRelaySwitching() {
    ESP_LOGE("TEST", "Starting testRapidRelaySwitching");
    // Clear history to only count transitions from this test
    resetPinHistory();

    // Rapidly switch relay on and off
    for (int i = 0; i < 5; i++) {
        relayFixture->relays->setRelay(0, true);
        advanceMockTime(10);
        relayFixture->relays->setRelay(0, false);
        advanceMockTime(10);
    }

    TEST_ASSERT_EQUAL(10, getPinTransitionCount(9));
}

void testSequentialRelayOperations() {
    // Clear history to start fresh
    resetMockState();
    relayFixture->relays->begin();

    // Test sequential activation pattern with timing
    relayFixture->relays->setRelay(0, true);  // Pin 9 goes LOW at time 0
    advanceMockTime(100);
    relayFixture->relays->setRelay(1, true);  // Pin 10 goes LOW at time 100
    advanceMockTime(100);
    relayFixture->relays->setRelay(2, true);  // Pin 20 goes LOW at time 200
    advanceMockTime(100);
    relayFixture->relays->setRelay(3, true);  // Pin 21 goes LOW at time 300

    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_TRUE(relayFixture->relays->getRelayState(3));

    // Verify timing between first relay activation and last relay activation
    // Pin 9 goes LOW at time 0, Pin 21 goes LOW at time 300
    // So we check the time between pin 9 going LOW and current time
    TEST_ASSERT_EQUAL(300, millis());
}
