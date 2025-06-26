#pragma once
#include <cstdint>

class IAudioController {
public:
    enum Sound : uint8_t {
        SOUND_STARTUP = 1,
        SOUND_WAITING = 2,
        SOUND_ACCEPTED = 3,
        SOUND_DENIED_1 = 4,
        SOUND_DENIED_2 = 5,
        SOUND_DENIED_3 = 6
        // Add other sounds as needed
    };

    virtual bool begin() = 0;
    virtual void setVolume(uint8_t volume) = 0;
    virtual void playTrack(uint8_t track) const = 0;
    virtual void reset() const = 0;
    virtual uint8_t getStatus() const = 0;
    virtual uint8_t getVolume() const = 0;
    virtual uint16_t getCurrentPosition() const = 0;
    virtual void setSource(uint8_t source) = 0;
    virtual ~IAudioController() = default;
};
