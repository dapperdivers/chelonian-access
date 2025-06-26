#include <unity.h>
#include "test_helpers.h"

// External fixtures from test_helpers.cpp
extern AudioTestFixture* audioFixture;

void testAudioInitialization() {
    TEST_ASSERT_TRUE(audioFixture->mock->initialized);
}

void testAudioPlayTracks() {
    audioFixture->audio->playTrack(IAudioController::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(IAudioController::SOUND_ACCEPTED, audioFixture->mock->getLastPlayedTrack());
}

void testAudioSetVolume() {
    audioFixture->audio->setVolume(10);
    TEST_ASSERT_EQUAL(10, audioFixture->mock->getVolume());
}

void testAudioReset() {
    audioFixture->audio->reset();
    // Reset should clear the player state
    TEST_ASSERT_EQUAL(MP3_STATUS_STOPPED, audioFixture->mock->getStatus());
    TEST_ASSERT_EQUAL(0, audioFixture->mock->getCurrentPosition());
}

void testAudioStatusMonitoring() {
    TEST_ASSERT_EQUAL(MP3_STATUS_STOPPED, audioFixture->audio->getStatus());

    audioFixture->audio->playTrack(IAudioController::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(MP3_STATUS_PLAYING, audioFixture->audio->getStatus());

    audioFixture->audio->setVolume(25);
    TEST_ASSERT_EQUAL(25, audioFixture->audio->getVolume());

    TEST_ASSERT_EQUAL(0, audioFixture->audio->getCurrentPosition());

    audioFixture->mock->setPosition(10);
    TEST_ASSERT_EQUAL(10, audioFixture->audio->getCurrentPosition());
}

// Additional tests

void testAudioSetVolumeClamping() {
    audioFixture->audio->begin();
    audioFixture->audio->setVolume(35);  // Above max
    TEST_ASSERT_EQUAL(30, audioFixture->mock->getVolume());

    audioFixture->audio->setVolume(0);  // Min
    TEST_ASSERT_EQUAL(0, audioFixture->mock->getVolume());
}

void testAudioPlayTrackWithoutInit() {
    // Create a new mock that hasn't been initialized
    MockAudioController uninitializedMock;
    uninitializedMock.playTrack(IAudioController::SOUND_ACCEPTED);
    // Without init, track should not be played
    TEST_ASSERT_EQUAL(0, uninitializedMock.getLastPlayedTrack());
}

void testAudioSetSourceValid() {
    audioFixture->audio->begin();
    audioFixture->audio->setSource(MP3_SRC_SDCARD);
    TEST_ASSERT_EQUAL(MP3_SRC_SDCARD, audioFixture->mock->getSource());

    audioFixture->audio->setSource(MP3_SRC_BUILTIN);
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, audioFixture->mock->getSource());
}

void testAudioGetVolumeWithoutInit() {
    MockAudioController uninitializedMock;
    // Default volume should be returned
    TEST_ASSERT_EQUAL(20, uninitializedMock.getVolume());
}

void testAudioGetStatusWithoutInit() {
    MockAudioController uninitializedMock;
    TEST_ASSERT_EQUAL(MP3_STATUS_STOPPED, uninitializedMock.getStatus());
}

void testAudioGetCurrentPositionWithoutInit() {
    MockAudioController uninitializedMock;
    TEST_ASSERT_EQUAL(0, uninitializedMock.getCurrentPosition());
}

void testAudioMultipleTracksSequence() {
    audioFixture->audio->begin();

    // Play multiple tracks in sequence
    audioFixture->audio->playTrack(IAudioController::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(IAudioController::SOUND_STARTUP, audioFixture->mock->getLastPlayedTrack());

    audioFixture->audio->playTrack(IAudioController::SOUND_WAITING);
    TEST_ASSERT_EQUAL(IAudioController::SOUND_WAITING, audioFixture->mock->getLastPlayedTrack());

    audioFixture->audio->playTrack(IAudioController::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(IAudioController::SOUND_ACCEPTED, audioFixture->mock->getLastPlayedTrack());
}

// Test function aliases to match main.cpp declarations
void testAudioVolumeControl() {
    testAudioSetVolume();
    testAudioSetVolumeClamping();
}

void testAudioTrackConstants() {
    // Test that track constants are properly defined
    TEST_ASSERT_EQUAL(1, IAudioController::SOUND_STARTUP);
    TEST_ASSERT_EQUAL(2, IAudioController::SOUND_WAITING);
    TEST_ASSERT_EQUAL(3, IAudioController::SOUND_ACCEPTED);
    TEST_ASSERT_EQUAL(4, IAudioController::SOUND_DENIED_1);
    TEST_ASSERT_EQUAL(5, IAudioController::SOUND_DENIED_2);
    TEST_ASSERT_EQUAL(6, IAudioController::SOUND_DENIED_3);
}

void testAudioSourceControl() {
    testAudioSetSourceValid();
}

void testAudioSourceEnsuresBuiltinOnInit() {
    audioFixture->audio->begin();
    // Should default to builtin source
    TEST_ASSERT_EQUAL(MP3_SRC_BUILTIN, audioFixture->mock->getSource());
}
