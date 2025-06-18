# Unit Test Update Summary

## Overview
The codebase has been refactored with the following key changes that require test updates:

1. **Relay Controller**: New GPIO pin mappings for ESP32-C3
2. **Main Logic**: Sequential relay activation instead of simultaneous
3. **Audio Player**: Minor implementation details (mostly backward compatible)
4. **RFID Controller**: Dynamic PN532 allocation (tests should work as-is)

## Critical Test Changes Required

### 1. Relay Controller Tests (`test_relay_controller.cpp`)
**GPIO Pin Mapping Changes:**
- Relay 2: GPIO 6 → GPIO 10
- Relay 3: GPIO 5 → GPIO 20
- Relay 4: GPIO 4 → GPIO 21

**All occurrences of these pins need updating throughout the test file.**

### 2. Integration Tests (`test_integration.cpp`)
**Major Logic Changes:**
- Remove 10-second door unlock duration
- Implement sequential relay activation:
  - Relay 1 ON for 1 second
  - Relay 1 OFF, Relay 2 ON for 1 second
  - Relay 2 OFF (sequence complete)

**Key Constants to Update:**
```cpp
// Old:
constexpr uint32_t DOOR_UNLOCK_DURATION = 10000;  // 10 seconds
constexpr uint8_t DOOR_UNLOCK_RELAY = 0;

// New:
constexpr uint32_t RELAY1_DURATION = 1000;  // 1 second
constexpr uint32_t RELAY2_DURATION = 1000;  // 1 second
```

### 3. Audio Player Tests (`test_audio_controller.cpp`)
**No critical changes required** - existing tests should work with the updated implementation.

### 4. RFID Controller Tests (`test_rfid_controller.cpp`)
**No changes required** - dynamic allocation is handled internally.

## Implementation Priority
1. Update relay GPIO pins (simple find/replace)
2. Rewrite integration test logic for sequential relay activation
3. Verify all tests pass with updated code

## Next Steps
Switch to code mode to implement these changes in the test files.
