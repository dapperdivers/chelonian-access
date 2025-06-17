#include "audio_player.h"

// Create HardwareSerial instance for UART1 on ESP32-C3
HardwareSerial audioSerial(1);

AudioPlayer::AudioPlayer(uint8_t rx_pin, uint8_t tx_pin)
    : m_rx_pin(rx_pin),
      m_tx_pin(tx_pin)
#ifdef USINGMP3
      ,
      serial(&audioSerial)
#endif
{
}

bool AudioPlayer::begin() {
#ifdef USINGMP3
    // Initialize UART1 with JQ6500 communication parameters
    audioSerial.begin(9600, SERIAL_8N1, m_rx_pin, m_tx_pin);
    delay(500);

    // Create JQ6500 instance with our serial connection
    player = new JQ6500Serial(audioSerial);

    reset();  // Reset JQ6500 on startup
    delay(500);

    // Ensure we're using built-in flash memory (not SD card)
    setSource(MP3_SRC_BUILTIN);
    delay(100);

    player->setVolume(m_current_volume);

    audio_enabled = true;
    m_initialized = true;
    return true;
#else
    m_initialized = true;
    return true;
#endif
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

#ifdef USINGMP3
    if (audio_enabled) {
        player->setVolume(m_current_volume);
    }
#endif
}

void AudioPlayer::playTrack(uint8_t track) const {
    if (!m_initialized) {
        return;
    }

#ifdef USINGMP3
    if (audio_enabled) {
        player->playFileByIndexNumber(track);
    }
#endif
}

void AudioPlayer::reset() const {
#ifdef USINGMP3
    if (player != nullptr) {
        player->reset();
    }
#endif
}

uint8_t AudioPlayer::getStatus() const {
#ifdef USINGMP3
    if (!m_initialized || !audio_enabled) {
        return MP3_STATUS_STOPPED;
    }

    uint8_t status = player->getStatus();
    return status;
#else
    return MP3_STATUS_STOPPED;
#endif
}

uint8_t AudioPlayer::getVolume() const {
#ifdef USINGMP3
    if (!m_initialized || !audio_enabled) {
        return 0;
    }

    // Return cached value as getVolume() from JQ6500 can be unreliable
    return m_current_volume;
#else
    return 0;
#endif
}

uint16_t AudioPlayer::getCurrentPosition() const {
#ifdef USINGMP3
    if (!m_initialized || !audio_enabled) {
        return 0;
    }

    return player->currentFilePositionInSeconds();
#else
    return 0;
#endif
}

void AudioPlayer::setSource(uint8_t source) {
#ifdef USINGMP3
    if (!m_initialized || !audio_enabled) {
        return;
    }

    // Only allow built-in flash or SD card sources
    if (source == MP3_SRC_BUILTIN || source == MP3_SRC_SDCARD) {
        m_current_source = source;
        player->setSource(source);
    }
#endif
}

uint8_t AudioPlayer::getSource() const {
    // Return our cached source value since JQ6500 doesn't provide a getSource method
    return m_current_source;
}
