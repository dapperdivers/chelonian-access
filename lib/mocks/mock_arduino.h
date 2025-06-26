#pragma once

#include <array>
#include <cstdint>
#include <cstdio>

// Mock Arduino types and constants
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define DEC 10
#define HEX 16

// ESP32 Serial constants
#ifndef SERIAL_8N1
#define SERIAL_8N1 0x800001c
#endif

// Type definitions
using millis_t = unsigned long;
using boolean = bool;

// Maximum number of state changes to track
#define MAX_PIN_HISTORY 50
#define MAX_PINS 22  // Increased to support GPIO 20 and 21 on ESP32-C3

// Macro definitions
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#define F(string_literal) (string_literal)

// Pin state change record
struct PinStateChange {
    uint8_t pin;
    uint8_t state;
    millis_t timestamp;
};

// Mock HardwareSerial for unit testing (ESP32 pattern)
class HardwareSerial {
public:
    HardwareSerial(int uart_nr = 0) : m_uart_nr(uart_nr) {}

    // ESP32-style begin with config and pins
    void begin(unsigned long baud, uint32_t /*config*/ = SERIAL_8N1, int8_t rxPin = -1,
               int8_t txPin = -1) {
        m_baud_rate = baud;
        m_rx_pin = rxPin;
        m_tx_pin = txPin;
    }

    // Basic serial functions
    static void print(const char* str) {
        printf("[Serial print] %s", str);
    }
    static void print(const uint32_t VAL, int base = DEC) {
        if (base == HEX) {
            printf("[Serial print] 0x%x", VAL);
        } else {
            printf("[Serial print] %u", VAL);
        }
    }
    static void print(int val) {
        printf("[Serial print] %d", val);
    }
    static void println(const char* str) {
        printf("[Serial println] %s\n", str);
    }
    static void println(const uint32_t VAL, int base = DEC) {
        if (base == HEX) {
            printf("[Serial println] 0x%x\n", VAL);
        } else {
            printf("[Serial println] %u\n", VAL);
        }
    }
    static void println(int val) {
        printf("[Serial println] %d\n", val);
    }
    static void println() {
        printf("\n");
    }

    // Additional serial functions
    void write(uint8_t data) { /* Mock write */ }
    static int available() {
        return 0;
    }
    static int read() {
        return -1;
    }

    // Getters for testing
    unsigned long getBaudRate() const {
        return m_baud_rate;
    }
    int8_t getRxPin() const {
        return m_rx_pin;
    }
    int8_t getTxPin() const {
        return m_tx_pin;
    }

private:
    int m_uart_nr;
    unsigned long m_baud_rate = 0;
    int8_t m_rx_pin = -1;
    int8_t m_tx_pin = -1;
};

// Mock SoftwareSerial for audio on ESP32-C3
class SoftwareSerial {
public:
    SoftwareSerial(int8_t rxPin, int8_t txPin)
        : m_rx(rxPin), m_tx(txPin), m_baud(0), m_begun(false) {}
    void begin(unsigned long baud) {
        m_baud = baud;
        m_begun = true;
    }
    void end() {
        m_begun = false;
    }
    int available() {
        return 0;
    }
    int read() {
        return -1;
    }
    size_t write(uint8_t b) {
        // Optionally track written bytes for test
        return 1;
    }
    void flush() {}
    bool isBegun() const {
        return m_begun;
    }
    unsigned long getBaud() const {
        return m_baud;
    }
    int8_t getRx() const {
        return m_rx;
    }
    int8_t getTx() const {
        return m_tx;
    }

private:
    int8_t m_rx;
    int8_t m_tx;
    unsigned long m_baud;
    bool m_begun;
};

// Global Serial object
extern HardwareSerial Serial;
extern SoftwareSerial audioSerial;  // For ESP32-C3, using SoftwareSerial for audio

// Mock state tracking
extern std::array<uint8_t, MAX_PINS> mockPinModes;
extern std::array<uint8_t, MAX_PINS> mockPinStates;
extern millis_t mockMillis;
extern std::array<PinStateChange, MAX_PIN_HISTORY> mockPinHistory;
extern uint8_t mockHistoryIndex;

// Mock Arduino functions
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t state);
uint8_t digitalRead(uint8_t pin);
void delay(unsigned long ms);
millis_t millis();

// Test helper functions
void resetMockState();
uint8_t getPinState(uint8_t pin);
uint8_t getPinMode(uint8_t pin);
void setMockMillis(millis_t value);
uint8_t getPinHistoryCount();
PinStateChange getPinHistoryEntry(uint8_t index);
void resetPinHistory();  // Reset only pin state change history
