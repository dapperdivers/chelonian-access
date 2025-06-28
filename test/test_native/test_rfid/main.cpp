#include <test_helpers.h>  // Provides setUp() and tearDown()
#include <unity.h>
#include "test_rfid_controller.h"  // Include the test function declarations

int main(void) {
    UNITY_BEGIN();

    // Run your test functions
    RUN_TEST(testRfidInitialization);
    RUN_TEST(testRfidDefaultUids);
    RUN_TEST(testRfidAddUids);
    RUN_TEST(testRfidInvalidUidLength);
    RUN_TEST(testRfidMaxUidLimit);

    return UNITY_END();
}
