#include <unity.h>
#include "relay_controller.h"  // Include for pin definitions
#include "test_helpers.h"

// External fixtures from test_helpers.cpp
extern RelayTestFixture* relayFixture;

void testRelayInitialState() {
    // Test that relays are in initial state (all off)
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(0));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(1));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(2));
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(3));
}

void testSetSingleRelay() {
    // Clear pin history before the test
    resetPinHistory();

    // Turn relay 0 on
    relayFixture->relays->setRelay(0, true);

    // Verify the pin state changed and was recorded
    TEST_ASSERT_EQUAL(1, getPinHistoryCount());
    PinStateChange change1 = getPinHistoryEntry(0);

    // Debug print with clearer labels
    printf("DEBUG: Pin History Entry 0 - Pin: %d, State: %d, Timestamp: %lu\n", change1.pin,
           change1.state, change1.timestamp);
    printf("DEBUG: Expected Pin for Relay 0: %d\n", RELAY1_PIN);

    // Assert that the recorded pin is RELAY1_PIN
    TEST_ASSERT_EQUAL(RELAY1_PIN, change1.pin);

    // Assert the state
    TEST_ASSERT_EQUAL(LOW, change1.state);  // Active LOW

    // Turn relay 0 off
    relayFixture->relays->setRelay(0, false);

    // Verify the pin state changed again and was recorded
    TEST_ASSERT_EQUAL(2, getPinHistoryCount());
    PinStateChange change2 = getPinHistoryEntry(1);
    TEST_ASSERT_EQUAL(RELAY1_PIN, change2.pin);
    TEST_ASSERT_EQUAL(HIGH, change2.state);  // Active LOW
}

void testSetAllRelays() {
    // Clear pin history before the test
    resetPinHistory();

    // Turn all relays on
    relayFixture->relays->setAllRelays(true);

    // Verify each relay pin was set to LOW (active)
    TEST_ASSERT_EQUAL(4, getPinHistoryCount());
    // Note: The order of pin changes might not be guaranteed,
    // but we can check if each relay pin was set to LOW.
    // A more robust test would check for specific pin/state pairs.
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY1_PIN));
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY2_PIN));
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY3_PIN));
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY4_PIN));

    // Clear pin history for the next part
    resetPinHistory();

    // Turn all relays off
    relayFixture->relays->setAllRelays(false);

    // Verify each relay pin was set to HIGH (inactive)
    TEST_ASSERT_EQUAL(4, getPinHistoryCount());
    TEST_ASSERT_EQUAL(HIGH, getPinState(RELAY1_PIN));
    TEST_ASSERT_EQUAL(HIGH, getPinState(RELAY2_PIN));
    TEST_ASSERT_EQUAL(HIGH, getPinState(RELAY3_PIN));
    TEST_ASSERT_EQUAL(HIGH, getPinState(RELAY4_PIN));
}

void testInvalidRelayNumber() {
    // No pin history changes should occur for invalid relay numbers
    resetPinHistory();

    // Test invalid relay number
    TEST_ASSERT_FALSE(relayFixture->relays->getRelayState(4));

    // Try to set invalid relay
    relayFixture->relays->setRelay(4, true);

    // Verify no pin history changes occurred
    TEST_ASSERT_EQUAL(0, getPinHistoryCount());
}

void testRelayStateTransitions() {
    // Clear pin history before the test
    resetPinHistory();

    // Test sequence: OFF -> ON -> OFF
    relayFixture->relays->setRelay(0, false);
    relayFixture->relays->setRelay(0, true);
    relayFixture->relays->setRelay(0, false);

    // Verify state transitions using pin history
    TEST_ASSERT_EQUAL(3, getPinHistoryCount());

    PinStateChange change1 = getPinHistoryEntry(0);
    TEST_ASSERT_EQUAL(RELAY1_PIN, change1.pin);
    TEST_ASSERT_EQUAL(HIGH, change1.state);  // Active LOW

    PinStateChange change2 = getPinHistoryEntry(1);
    TEST_ASSERT_EQUAL(RELAY1_PIN, change2.pin);
    TEST_ASSERT_EQUAL(LOW, change2.state);  // Active LOW

    PinStateChange change3 = getPinHistoryEntry(2);
    TEST_ASSERT_EQUAL(RELAY1_PIN, change3.pin);
    TEST_ASSERT_EQUAL(HIGH, change3.state);  // Active LOW
}

void testRapidRelaySwitching() {
    // Clear pin history before the test
    resetPinHistory();

    // Rapidly switch relay on and off
    for (int i = 0; i < 5; i++) {
        relayFixture->relays->setRelay(0, true);
        advanceMockTime(10);
        relayFixture->relays->setRelay(0, false);
        advanceMockTime(10);
    }

    // Verify we have 10 state changes recorded
    TEST_ASSERT_EQUAL(10, getPinHistoryCount());
}

void testSequentialRelayOperations() {
    // Clear pin history before the test
    resetPinHistory();

    // Test sequential activation pattern with timing
    millis_t startTime = millis();

    relayFixture->relays->setRelay(0, true);
    advanceMockTime(100);

    relayFixture->relays->setRelay(1, true);
    advanceMockTime(100);

    relayFixture->relays->setRelay(2, true);
    advanceMockTime(100);

    relayFixture->relays->setRelay(3, true);

    // Verify all relays were activated in sequence using pin history
    TEST_ASSERT_EQUAL(4, getPinHistoryCount());

    // Verify relay numbers and states (Active LOW)
    TEST_ASSERT_EQUAL(RELAY1_PIN, getPinHistoryEntry(0).pin);
    TEST_ASSERT_EQUAL(LOW, getPinHistoryEntry(0).state);
    TEST_ASSERT_EQUAL(RELAY2_PIN, getPinHistoryEntry(1).pin);
    TEST_ASSERT_EQUAL(LOW, getPinHistoryEntry(1).state);
    TEST_ASSERT_EQUAL(RELAY3_PIN, getPinHistoryEntry(2).pin);
    TEST_ASSERT_EQUAL(LOW, getPinHistoryEntry(2).state);
    TEST_ASSERT_EQUAL(RELAY4_PIN, getPinHistoryEntry(3).pin);
    TEST_ASSERT_EQUAL(LOW, getPinHistoryEntry(3).state);

    // Verify timing (approximate due to advanceMockTime)
    TEST_ASSERT_TRUE(millis() - startTime >= 300);
}

void testMultipleRelayControl() {
    // Clear pin history before the test
    resetPinHistory();

    // Control multiple relays
    relayFixture->relays->setRelay(0, true);
    relayFixture->relays->setRelay(2, true);
    relayFixture->relays->setRelay(1, false);
    relayFixture->relays->setRelay(3, true);

    // Verify operations using pin history
    TEST_ASSERT_EQUAL(4, getPinHistoryCount());

    // Verify specific pin state changes (Active LOW)
    // Note: Order might not be guaranteed, but we can check for presence and state
    // A more robust test would check the exact sequence if order is important.
    // For now, we check if the pins were set to the expected states.
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY1_PIN));   // Relay 0 ON
    TEST_ASSERT_EQUAL(HIGH, getPinState(RELAY2_PIN));  // Relay 1 OFF
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY3_PIN));   // Relay 2 ON
    TEST_ASSERT_EQUAL(LOW, getPinState(RELAY4_PIN));   // Relay 3 ON
}
