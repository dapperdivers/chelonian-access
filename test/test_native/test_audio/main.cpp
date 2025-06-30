#include <test_helpers.h>  // Provides setUp() and tearDown()
#include <unity.h>
#include "test_audio_player.h"  // Include the test function declarations

int main(void) {
    UNITY_BEGIN();

    // Run your test functions
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
