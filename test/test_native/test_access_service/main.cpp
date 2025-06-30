#include <test_helpers.h>  // Provides setUp() and tearDown()
#include <unity.h>
#include "test_access_service.h"  // Include the test function declarations

int main(void) {
    UNITY_BEGIN();

    // Run your test functions
    RUN_TEST(testInitialState);
    RUN_TEST(testActivateRelays);
    RUN_TEST(testRelaySequence);
    RUN_TEST(testImpatientWaiting);
    RUN_TEST(testInvalidCardDelays);

    return UNITY_END();
}
