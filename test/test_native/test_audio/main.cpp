#include <unity.h>
#include "test_helpers.h"

// External fixture from test_helpers.cpp
extern AudioTestFixture* audioFixture;

// External base functions from test_helpers.cpp
extern void baseSetUp();
extern void baseTearDown();

// Declare test functions
void testAudioInitialization();
void testAudioVolumeControl();
void testAudioPlayTracks();
void testAudioTrackConstants();
void testAudioReset();
void testAudioStatusMonitoring();
void testAudioSourceControl();
void testAudioSourceEnsuresBuiltinOnInit();

// Audio-specific setUp and tearDown
void setUp() {
    baseSetUp();
    audioFixture = new AudioTestFixture();

    // Initialize the audio mock
    if (audioFixture && audioFixture->audio) {
        audioFixture->audio->begin();
    }
}

void tearDown() {
    // Delete the fixture before calling baseTearDown
    if (audioFixture) {
        delete audioFixture;
        audioFixture = nullptr;
    }
    baseTearDown();
}

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testAudioInitialization);
    RUN_TEST(testAudioVolumeControl);
    RUN_TEST(testAudioPlayTracks);
    RUN_TEST(testAudioTrackConstants);
    RUN_TEST(testAudioReset);
    RUN_TEST(testAudioStatusMonitoring);
    RUN_TEST(testAudioSourceControl);
    RUN_TEST(testAudioSourceEnsuresBuiltinOnInit);

    return UNITY_END();
}
