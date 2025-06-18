#include "audio_controller.h"

AudioPlayer::AudioPlayer(uint8_t rx_pin, uint8_t tx_pin) : m_rx_pin(rx_pin), m_tx_pin(tx_pin) {}

bool AudioPlayer::begin() {
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
        return true;
    }
    return false;
}

void AudioPlayer::setVolume(uint8_t volume) {
    if (!m_initialized) {
        return;
    }

    // Clamp volume between 0-30
    if (volume > 30) {
        m_current_volume = 30;
    } else {
        m_current_volume = volume;
    }

    if (audio_enabled) {
        player->setVolume(m_current_volume);
    }
}

void AudioPlayer::playTrack(uint8_t track) const {
    if (!m_initialized) {
        return;
    }

    if (audio_enabled) {
        player->playFileByIndexNumber(track);
    }
}

void AudioPlayer::reset() const {
    if (player != nullptr) {
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
