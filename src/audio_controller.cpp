#include "audio_controller.h"

#ifdef UNIT_TEST
#include "mock_arduino.h"
#include "mock_esp_log.h"
#else
#include <Arduino.h>
#include "esp_log.h"
#endif

static const char* TAG = "Audio";

AudioPlayer::AudioPlayer(uint8_t rx_pin, uint8_t tx_pin) : m_rx_pin(rx_pin), m_tx_pin(tx_pin) {}

AudioPlayer::~AudioPlayer() {
    delete audioSerial;
    delete player;
}

bool AudioPlayer::begin() {
    ESP_LOGI(TAG, "%lu - Initializing audio controller...", millis());
    if (audioSerial == nullptr) {
        audioSerial = new SoftwareSerial(m_rx_pin, m_tx_pin);
    }
    audioSerial->begin(9600);  // Only pass baud for SoftwareSerial
    if (player == nullptr) {
        player = new JQ6500_Serial(m_rx_pin, m_tx_pin);
    }

    if (player != nullptr) {
        reset();  // Reset JQ6500 on startup
        delay(500);

        // Ensure we're using built-in flash memory (not SD card)
        setSource(MP3_SRC_BUILTIN);
        delay(100);

        player->setVolume(m_current_volume);

        audio_enabled = true;
        m_initialized = true;
        ESP_LOGI(TAG, "%lu - Initialized successfully. Volume: %d, Source: %s", millis(),
                 m_current_volume, m_current_source == MP3_SRC_BUILTIN ? "Built-in" : "SD Card");
        return true;
    }
    ESP_LOGE(TAG, "%lu - Initialization failed!", millis());
    return false;
}

void AudioPlayer::setVolume(uint8_t volume) {
    if (!m_initialized) {
        return;
    }
    ESP_LOGI(TAG, "%lu - Volume changing from %d to ", millis(), m_current_volume);

    // Clamp volume between 0-30
    if (volume > 30) {
        m_current_volume = 30;
    } else {
        m_current_volume = volume;
    }

    if (audio_enabled) {
        player->setVolume(m_current_volume);
        ESP_LOGI(TAG, "%d", m_current_volume);
    } else {
        ESP_LOGI(TAG, " (audio disabled)");
    }
}

void AudioPlayer::playTrack(uint8_t track) {
    if (!m_initialized) {
        return;
    }
    ESP_LOGI(TAG, "%lu - Playing track %d", millis(), track);

    if (audio_enabled) {
        player->playFileByIndexNumber(track);
        m_last_played_track = track;
    }
}

void AudioPlayer::reset() {
    if (player != nullptr) {
        ESP_LOGI(TAG, "%lu - Resetting audio player", millis());
        player->reset();
    }
}

uint8_t AudioPlayer::getStatus() const {
    if (!m_initialized || !audio_enabled) {
        return MP3_STATUS_STOPPED;
    }

    uint8_t status = player->getStatus();
    return status;
}

uint8_t AudioPlayer::getVolume() const {
    if (!m_initialized || !audio_enabled) {
        return 0;
    }

    // Return cached value as getVolume() from JQ6500 can be unreliable
    return m_current_volume;
}

uint16_t AudioPlayer::getCurrentPosition() const {
    if (!m_initialized || !audio_enabled) {
        return 0;
    }

    return player->currentFilePositionInSeconds();
}

void AudioPlayer::setSource(uint8_t source) {
    if (!m_initialized || !audio_enabled) {
        return;
    }

    // Only allow built-in flash or SD card sources
    if (source == MP3_SRC_BUILTIN || source == MP3_SRC_SDCARD) {
        m_current_source = source;
        player->setSource(source);
    }
}

uint8_t AudioPlayer::getSource() const {
    // Return our cached source value since JQ6500 doesn't provide a getSource method
    return m_current_source;
}

uint8_t AudioPlayer::getLastPlayedTrack() const {
    return m_last_played_track;
}

#ifdef UNIT_TEST
void AudioPlayer::setPosition(uint16_t seconds) {
    if (!m_initialized || !audio_enabled) {
        return;
    }
    player->setPosition(seconds);
}
#endif
