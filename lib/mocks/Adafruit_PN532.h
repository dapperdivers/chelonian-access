#pragma once

#ifdef UNIT_TEST
#include "mock_pn532.h"  // Contains mock Adafruit_PN532
#else
#include <Adafruit_PN532.h>  // Real Adafruit_PN532 library
#endif
