#pragma once

#include <cstdint>
#include "mock_arduino.h"  // Include this to get HardwareSerial definition

// JQ6500 Status constants (matching the real library)
#ifndef MP3_STATUS_STOPPED
#define MP3_STATUS_STOPPED 0
#define MP3_STATUS_PLAYING 1
#define MP3_STATUS_PAUSED 2
#endif

// JQ6500 Source constants
#ifndef MP3_SRC_BUILTIN
#define MP3_SRC_BUILTIN 0
#define MP3_SRC_SDCARD 1
#endif

// Mock JQ6500_Serial class for unit testing
class JQ6500_Serial {
public:
    JQ6500_Serial(HardwareSerial& serial) : m_serial(serial) {}

    void reset() {
        // Mock reset functionality
        m_is_playing = false;
        m_status = MP3_STATUS_STOPPED;
        m_position_seconds = 0;
    }

    void setVolume(uint8_t volume) {
        m_volume = volume;
    }

    uint8_t getVolume() const {
        return m_volume;
    }

    void playFileByIndexNumber(uint8_t fileNumber) {
        m_last_played_track = fileNumber;
        m_is_playing = true;
        m_status = MP3_STATUS_PLAYING;
        m_position_seconds = 0;
    }

    bool isPlaying() const {
        return m_is_playing;
    }

    uint8_t getLastPlayedTrack() const {
        return m_last_played_track;
    }

    // Status monitoring methods
    uint8_t getStatus() const {
        return m_status;
    }

    uint16_t currentFilePositionInSeconds() const {
        return m_position_seconds;
    }

    // Source control methods
    void setSource(uint8_t source) {
        m_source = source;
    }

    uint8_t getSource() const {
        return m_source;
    }

    // Test helper methods
    void setStatus(uint8_t status) {
        m_status = status;
        m_is_playing = (status == MP3_STATUS_PLAYING);
    }

    void setPosition(uint16_t seconds) {
        m_position_seconds = seconds;
    }

private:
    HardwareSerial& m_serial;
    uint8_t m_volume{20};
    bool m_is_playing{false};
    uint8_t m_last_played_track{0};
    uint8_t m_status{MP3_STATUS_STOPPED};
    uint8_t m_source{MP3_SRC_BUILTIN};
    uint16_t m_position_seconds{0};
};
