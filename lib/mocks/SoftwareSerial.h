#pragma once

#ifdef UNIT_TEST
#include "mock_arduino.h"  // Contains mock SoftwareSerial
#else
#include <SoftwareSerial.h>  // Real SoftwareSerial library
#endif
