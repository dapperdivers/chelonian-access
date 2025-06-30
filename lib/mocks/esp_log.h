#pragma once

#ifdef UNIT_TEST
#include "mock_esp_log.h"  // Contains mock esp_log functions
#else
#include <esp_log.h>  // Real ESP-IDF logging header
#endif
