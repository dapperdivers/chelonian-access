#ifndef TEST_RFID_CONTROLLER_H
#define TEST_RFID_CONTROLLER_H

#include <unity.h>

// Declare your test functions here
void testRfidInitialization();
void testRfidDefaultUids();
void testRfidAddUids();
void testRfidInvalidUidLength();
void testRfidMaxUidLimit();

#endif  // TEST_RFID_CONTROLLER_H
