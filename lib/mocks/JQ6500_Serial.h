#pragma once

#ifdef UNIT_TEST
#include "mock_jq6500.h"  // Contains mock JQ6500_Serial
#else
#include <JQ6500_Serial.h>  // Real JQ6500_Serial library
#endif
