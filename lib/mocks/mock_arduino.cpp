#include "mock_arduino.h"

// Mock state variables
std::array<uint8_t, MAX_PINS> mockPinModes{};
std::array<uint8_t, MAX_PINS> mockPinStates{};
std::array<PinStateChange, MAX_PIN_HISTORY> mockPinHistory{};
uint8_t mockHistoryIndex = 0;
millis_t mockMillis = 0;
millis_t mock_delay_time = 0;
HardwareSerial serial;

// Forward declarations
void recordPinStateChange(uint8_t pin, uint8_t state);

// Mock function implementations
void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < MAX_PINS) {
        mockPinModes[pin] = mode;
    }
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin < MAX_PINS) {
        mockPinStates[pin] = val;
        recordPinStateChange(pin, val);
    }
}

uint8_t digitalRead(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mockPinStates[pin];
    }
    return 0;
}

millis_t millis(void) {
    return mockMillis;
}

void delay(unsigned long ms) {
    mock_delay_time += ms;
    mockMillis += ms;
}

// Helper function implementations
void resetMockState(void) {
    for (int i = 0; i < MAX_PINS; i++) {
        mockPinModes[i] = 0;
        mockPinStates[i] = 0;
    }
    mockHistoryIndex = 0;
    mockMillis = 0;
    mock_delay_time = 0;
    // Serial doesn't need reset - it's stateless in our mock
}

uint8_t getPinState(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mockPinStates[pin];
    }
    return 0;
}

uint8_t getPinMode(uint8_t pin) {
    if (pin < MAX_PINS) {
        return mockPinModes[pin];
    }
    return 0;
}

void setMockMillis(millis_t value) {
    mockMillis = value;
}

uint8_t getPinHistoryCount() {
    return mockHistoryIndex;
}

PinStateChange getPinHistoryEntry(uint8_t index) {
    if (index < mockHistoryIndex) {
        return mockPinHistory[index];
    }
    return {0, 0, 0};
}

void resetPinHistory(void) {
    mockHistoryIndex = 0;
}

// Internal helper functions
void recordPinStateChange(uint8_t pin, uint8_t state) {
    if (mockHistoryIndex < MAX_PIN_HISTORY) {
        mockPinHistory[mockHistoryIndex].pin = pin;
        mockPinHistory[mockHistoryIndex].state = state;
        mockPinHistory[mockHistoryIndex].timestamp = mockMillis;
        mockHistoryIndex++;
    }
}
