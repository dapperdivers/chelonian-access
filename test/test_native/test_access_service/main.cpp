#include <unity.h>
#include "access_service.h"
#include "test_helpers.h"

// Declare test functions
void testInitialState();
void testActivateRelays();
void testRelaySequence();
void testImpatientWaiting();
void testInvalidCardDelays();

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testInitialState);
    RUN_TEST(testActivateRelays);
    RUN_TEST(testRelaySequence);
    RUN_TEST(testImpatientWaiting);
    RUN_TEST(testInvalidCardDelays);

    return UNITY_END();
}
