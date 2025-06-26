#pragma once
#include "IAudioController.h"
#include "mock_jq6500.h"

class MockAudioController : public IAudioController {
public:
    MockAudioController() : player(8, 7) {}

    bool begin() override {
        initialized = true;
        return true;
    }
    void setVolume(uint8_t volume) override {
        player.setVolume(volume);
    }
    void playTrack(uint8_t track) const override {
        player.playFileByIndexNumber(track);
    }
    void reset() const override {
        player.reset();
    }
    uint8_t getStatus() const override {
        return player.getStatus();
    }
    uint8_t getVolume() const override {
        return player.getVolume();
    }
    uint16_t getCurrentPosition() const override {
        return player.currentFilePositionInSeconds();
    }
    void setSource(uint8_t source) override {
        player.setSource(source);
    }
    uint8_t getSource() const {
        return player.getSource();
    }
    uint8_t getLastPlayedTrack() const {
        return player.getLastPlayedTrack();
    }
    void setPosition(uint16_t seconds) {
        player.setPosition(seconds);
    }

    mutable JQ6500_Serial player;
    bool initialized = false;
};
