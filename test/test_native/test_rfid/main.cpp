#include <unity.h>
#include "test_helpers.h"

// Declare test functions
void testRfidInitialization();
void testRfidDefaultUids();
void testRfidAddUids();
void testRfidInvalidUidLength();
void testRfidMaxUidLimit();

// setUp and tearDown are defined in test_helpers.cpp

int main(int /*argc*/, char** /*argv*/) {
    UNITY_BEGIN();

    RUN_TEST(testRfidInitialization);
    RUN_TEST(testRfidDefaultUids);
    RUN_TEST(testRfidAddUids);
    RUN_TEST(testRfidInvalidUidLength);
    RUN_TEST(testRfidMaxUidLimit);

    return UNITY_END();
}
