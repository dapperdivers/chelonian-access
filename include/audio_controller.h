#pragma once

#include "IAudioController.h"

#ifdef UNIT_TEST
#include "mock_arduino.h"
#ifdef USINGMP3
#include "mock_jq6500.h"
#endif
#else
#include <Arduino.h>
#include <SoftwareSerial.h>  // For SoftwareSerial on ESP32-C3
#ifdef USINGMP3
#include <JQ6500_Serial.h>
#endif
#endif

// JQ6500 Status constants
#define MP3_STATUS_STOPPED 0
#define MP3_STATUS_PLAYING 1
#define MP3_STATUS_PAUSED 2

// JQ6500 Source constants
#define MP3_SRC_BUILTIN 4
#define MP3_SRC_SDCARD 1

class AudioController : public IAudioController {
public:
    AudioController(uint8_t rx_pin = 0, uint8_t tx_pin = 1);  // ESP32-C3 Serial1 pins

    bool begin();
    void setVolume(uint8_t volume);
    void playTrack(uint8_t track) const;
    void reset() const;  // JQ6500 reset method

    // Status monitoring methods
    uint8_t getStatus() const;
    uint8_t getVolume() const;
    uint16_t getCurrentPosition() const;  // Current position in seconds

    // Source control methods
    void setSource(uint8_t source);
    uint8_t getSource() const;  // Note: JQ6500 doesn't actually have a getSource command

private:
    SoftwareSerial* audioSerial = nullptr;
    bool m_initialized{false};
    uint8_t m_current_volume{20};
    uint8_t m_rx_pin;
    uint8_t m_tx_pin;
    uint8_t m_current_source{
        MP3_SRC_BUILTIN};  // Track source internally since JQ6500 doesn't provide getSource

#ifdef USINGMP3
    // State for MP3 player
public:
    bool audio_enabled{false};
    JQ6500_Serial* player{nullptr};
#endif
};
