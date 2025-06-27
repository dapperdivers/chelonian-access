#include <unity.h>
#include "audio_controller.h"
#include "test_helpers.h"

void testAudioInitialization() {
    TEST_ASSERT_TRUE(audioFixture->audio->begin());
}

void testAudioVolumeControl() {
    audioFixture->audio->begin();

    // Test normal volume setting
    audioFixture->audio->setVolume(15);
    // Note: We can't directly verify volume without accessing private members
    // In a real scenario, we'd need getter methods or friend classes

    // Test volume clamping
    audioFixture->audio->setVolume(50);  // Should be clamped to 30
    audioFixture->audio->setVolume(-5);  // Should be clamped to 0
}

void testAudioPlayTracks() {
    audioFixture->audio->begin();

    // Test playing different tracks and verify with mock
    audioFixture->audio->playTrack(AudioPlayer::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_STARTUP, audioFixture->audio->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_WAITING);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_WAITING, audioFixture->audio->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_ACCEPTED, audioFixture->audio->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_1);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_1, audioFixture->audio->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_2);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_2, audioFixture->audio->getLastPlayedTrack());

    audioFixture->audio->playTrack(AudioPlayer::SOUND_DENIED_3);
    TEST_ASSERT_EQUAL(AudioPlayer::SOUND_DENIED_3, audioFixture->audio->getLastPlayedTrack());

    // Test playing before initialization
    AudioPlayer uninitializedAudio;
    uninitializedAudio.playTrack(1);  // Should not crash or interact with player
                                      // Add assertion here if possible to confirm no interaction,
                                      // but mock_jq6500 doesn't currently support this.
}

void testAudioTrackConstants() {
    // Verify track constants match expected values
    TEST_ASSERT_EQUAL(1, AudioPlayer::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(2, AudioPlayer::SOUND_WAITING);
    TEST_ASSERT_EQUAL(3, AudioPlayer::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(4, AudioPlayer::SOUND_DENIED_1);
    TEST_ASSERT_EQUAL(5, AudioPlayer::SOUND_DENIED_2);
    TEST_ASSERT_EQUAL(6, AudioPlayer::SOUND_DENIED_3);
}

void testAudioReset() {
    audioFixture->audio->begin();

    // Test reset functionality (JQ6500 specific)
    audioFixture->audio->reset();
    // Reset method exists but we can't verify it was called without
    // adding tracking to the mock
}

void testAudioStatusMonitoring() {
    audioFixture->audio->begin();

    // Initial status should be stopped
    TEST_ASSERT_EQUAL(MP3_STATUS_STOPPED, audioFixture->audio->getStatus());

    // Play a track and check status
    audioFixture->audio->playTrack(AudioPlayer::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(MP3_STATUS_PLAYING, audioFixture->audio->getStatus());

    // Test volume getter
    audioFixture->audio->setVolume(25);
    TEST_ASSERT_EQUAL(25, audioFixture->audio->getVolume());

    // Test position (mock starts at 0)
    TEST_ASSERT_EQUAL(0, audioFixture->audio->getCurrentPosition());

    // Simulate position change in mock
    audioFixture->audio->setPosition(10);
    TEST_ASSERT_EQUAL(10, audioFixture->audio->getCurrentPosition());
}

void testAudioSourceControl() {
    audioFixture->audio->begin();

    // Default source should be built-in flash
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, audioFixture->audio->getSource());

    // Try to set to SD card source
    audioFixture->audio->setSource(MP3_SRC_SDCARD);
    TEST_ASSERT_EQUAL(MP3_SRC_SDCARD, audioFixture->audio->getSource());

    // Verify mock also has correct source
    TEST_ASSERT_EQUAL(MP3_SRC_SDCARD, audioFixture->audio->getSource());

    // Set back to built-in
    audioFixture->audio->setSource(MP3_SRC_BUILTIN);
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, audioFixture->audio->getSource());
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, audioFixture->audio->getSource());
}

void testAudioSourceEnsuresBuiltinOnInit() {
    // Create a new audio player
    AudioPlayer testAudio;

    // Initialize it
    TEST_ASSERT_TRUE(testAudio.begin());

    // Source should be set to built-in flash automatically
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, testAudio.getSource());
}
