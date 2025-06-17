#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void testAudioInitialization();
void testAudioVolumeControl();
void testAudioPlayTracks();
void testAudioTrackConstants();
void testAudioReset();
void testAudioStatusMonitoring();
void testAudioSourceControl();
void testAudioSourceEnsuresBuiltinOnInit();

// setUp and tearDown are defined in test_helpers.cpp

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
