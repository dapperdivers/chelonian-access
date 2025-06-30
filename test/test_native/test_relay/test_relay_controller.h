#ifndef TEST_RELAY_CONTROLLER_H
#define TEST_RELAY_CONTROLLER_H

#include <unity.h>

// Declare your test functions here
void testRelayInitialState();
void testSetSingleRelay();
void testSetAllRelays();
void testInvalidRelayNumber();
void testRelayStateTransitions();
void testRapidRelaySwitching();
void testSequentialRelayOperations();

#endif  // TEST_RELAY_CONTROLLER_H
