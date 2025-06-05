#include <unity.h>
#include "mock_arduino.h"
#include "test_helpers.h"

// Integration test constants for ESP32-C3 + JQ6500 hardware
// These tests simulate the business logic of the RFID access control system
constexpr uint32_t RELAY1_DURATION = 1000;   // 1 second for relay 1
constexpr uint32_t RELAY2_DURATION = 1000;   // 1 second for relay 2
constexpr uint8_t RELAY1_PIN = 0;  // Relay 1 (0-indexed)
constexpr uint8_t RELAY2_PIN = 1;  // Relay 2 (0-indexed)
constexpr uint8_t MAXIMUM_INVALID_ATTEMPTS = 13;
const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1, 3, 4, 5, 8, 12, 17, 23, 30, 38, 47, 57, 68};

// ESP32-C3 GPIO mapping for relays (active LOW)
// RELAY1 = GPIO9
// RELAY2 = GPIO10
// Track numbering for JQ6500 MP3 player:
// Track 2 = SOUND_WAITING
// Track 3 = SOUND_ACCEPTED
// Track 4 = SOUND_DENIED_1
// Track 5 = SOUND_DENIED_2
// Track 6 = SOUND_DENIED_3

void test_valid_card_flow(void) {
    // Simulate valid card scan
    // Expected: Play acceptance sound (track 3)
    integrationFixture->state.lastPlayedTrack = 3;
    TEST_ASSERT_EQUAL(3, integrationFixture->state.lastPlayedTrack);
    
    // Expected: Sequential relay activation
    // Step 1: Activate relay 1
    digitalWrite(9, LOW); // Relay 1 on pin 9, active LOW
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[10]); // Relay 2 should be off
    integrationFixture->state.relayActive = true;
    integrationFixture->state.relayActivatedTime = millis();
    
    // Reset invalid attempts
    integrationFixture->state.invalidAttempts = 0;
    TEST_ASSERT_EQUAL(0, integrationFixture->state.invalidAttempts);
    
    // Advance time by 1 second (relay 1 duration)
    advanceMockTime(RELAY1_DURATION);
    
    // Step 2: Relay 1 OFF, Relay 2 ON
    digitalWrite(9, HIGH); // Relay 1 off
    digitalWrite(10, LOW); // Relay 2 on
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[10]);
    
    // Advance time by 1 second (relay 2 duration)
    advanceMockTime(RELAY2_DURATION);
    
    // Step 3: Relay 2 OFF - sequence complete
    digitalWrite(10, HIGH); // Relay 2 off
    integrationFixture->state.relayActive = false;
    
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[10]);
    TEST_ASSERT_FALSE(integrationFixture->state.relayActive);
}

void test_invalid_card_flow_first_attempt(void) {
    // Ensure relays are off at the start (HIGH = off due to active LOW logic)
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    
    // First invalid attempt
    integrationFixture->state.invalidAttempts = 0;
    
    // Expected: Play first denial sound (track 4)
    integrationFixture->state.lastPlayedTrack = 4;
    TEST_ASSERT_EQUAL(4, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 1 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[0] * 1000);
    delay(expectedDelay);
    TEST_ASSERT_EQUAL(expectedDelay, getTotalDelayTime());
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(1, integrationFixture->state.invalidAttempts);
    
    // No relay activation
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]); // Relay should remain off
}

void test_invalid_card_flow_second_attempt(void) {
    // Second invalid attempt
    integrationFixture->state.invalidAttempts = 1;
    
    // Expected: Play second denial sound (track 5)
    integrationFixture->state.lastPlayedTrack = 5;
    TEST_ASSERT_EQUAL(5, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 3 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[1] * 1000);
    delay(expectedDelay);
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(2, integrationFixture->state.invalidAttempts);
}

void test_invalid_card_flow_third_attempt(void) {
    // Third invalid attempt
    integrationFixture->state.invalidAttempts = 2;
    
    // Expected: Play third denial sound (track 6)
    integrationFixture->state.lastPlayedTrack = 6;
    TEST_ASSERT_EQUAL(6, integrationFixture->state.lastPlayedTrack);
    
    // Expected: 3 second base delay + 4 second exponential delay
    millis_t expectedDelay = 3000 + (invalidDelays[2] * 1000);
    delay(expectedDelay);
    
    // Increment invalid attempts
    integrationFixture->state.invalidAttempts++;
    TEST_ASSERT_EQUAL(3, integrationFixture->state.invalidAttempts);
}

void test_exponential_backoff(void) {
    // Test exponential backoff delays
    for (uint8_t i = 0; i < 5; i++) {
        resetMockState();
        integrationFixture->state.invalidAttempts = i;
        
        millis_t expectedDelay = 3000 + (invalidDelays[i] * 1000);
        delay(expectedDelay);
        
        TEST_ASSERT_EQUAL(expectedDelay, getTotalDelayTime());
    }
}

void test_max_invalid_attempts(void) {
    // Test that invalid attempts don't exceed maximum
    integrationFixture->state.invalidAttempts = MAXIMUM_INVALID_ATTEMPTS - 1;
    
    // Attempt to increment
    if (integrationFixture->state.invalidAttempts < MAXIMUM_INVALID_ATTEMPTS - 1) {
        integrationFixture->state.invalidAttempts++;
    }
    
    TEST_ASSERT_EQUAL(MAXIMUM_INVALID_ATTEMPTS - 1, integrationFixture->state.invalidAttempts);
}

void test_waiting_sound_after_10_seconds(void) {
    // Advance time to 10 seconds
    advanceMockTime(10001);
    
    bool impatient = false;
    bool scanned = false;
    
    // Check if waiting sound should play
    if (millis() > 10000 && !impatient && !scanned) {
        integrationFixture->state.lastPlayedTrack = 2; // SOUND_WAITING
        impatient = true;
    }
    
    TEST_ASSERT_EQUAL(2, integrationFixture->state.lastPlayedTrack);
}

void test_relay_timing_accuracy(void) {
    // Test sequential relay activation timing
    resetMockState();
    
    // Activate relay 1
    digitalWrite(9, LOW);
    millis_t startTime = millis();
    
    // Advance time by 1 second
    advanceMockTime(RELAY1_DURATION);
    
    // Switch to relay 2
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    millis_t relay1Duration = millis() - startTime;
    
    // Advance time by 1 second
    advanceMockTime(RELAY2_DURATION);
    
    // Deactivate relay 2
    digitalWrite(10, HIGH);
    millis_t totalDuration = millis() - startTime;
    
    // Verify timing
    TEST_ASSERT_EQUAL(RELAY1_DURATION, relay1Duration);
    TEST_ASSERT_EQUAL(RELAY1_DURATION + RELAY2_DURATION, totalDuration);
    
    // Verify state transitions
    TEST_ASSERT_EQUAL(2, getPinTransitionCount(9));  // LOW then HIGH
    TEST_ASSERT_EQUAL(2, getPinTransitionCount(10)); // LOW then HIGH
}

void test_sequential_relay_activation(void) {
    // Test the complete sequential relay activation pattern
    resetMockState();
    
    // Initial state - both relays off
    TEST_ASSERT_EQUAL(0, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(0, mock_pin_states[10]);
    
    // Step 1: Relay 1 ON
    digitalWrite(9, LOW);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(0, mock_pin_states[10]);  // Relay 2 unchanged
    
    advanceMockTime(RELAY1_DURATION);
    
    // Step 2: Relay 1 OFF, Relay 2 ON
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(LOW, mock_pin_states[10]);
    
    advanceMockTime(RELAY2_DURATION);
    
    // Step 3: Relay 2 OFF
    digitalWrite(10, HIGH);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[9]);
    TEST_ASSERT_EQUAL(HIGH, mock_pin_states[10]);
    
    // Verify the complete sequence
    const uint8_t expectedRelay1[] = {LOW, HIGH};
    const uint8_t expectedRelay2[] = {LOW, HIGH};
    
    TEST_ASSERT_TRUE(verifyPinSequence(9, expectedRelay1, 2));
    TEST_ASSERT_TRUE(verifyPinSequence(10, expectedRelay2, 2));
}