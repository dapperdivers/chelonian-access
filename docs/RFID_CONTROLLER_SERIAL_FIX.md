# RFID Controller Serial Fix Guide

## Problem Description
The rfid_controller.cpp file has compilation errors due to incorrect usage of HardwareSerial methods. The code is attempting to call print/println as static methods on the HardwareSerial class, but these are instance methods that must be called on the Serial object.

## Error Message
```
[Error] cannot call member function 'size_t Print::print(char)' without object
```

## Locations of Issues

### 1. Line 23 in `begin()` method:
```cpp
HardwareSerial::print(F("Didn't find PN53x board"));
```

### 2. Lines 74-79 in `printFirmwareVersion()` method:
```cpp
HardwareSerial::print(F("Found chip PN5"));
HardwareSerial::println((versiondata >> 24) & 0xFF, HEX);
HardwareSerial::print(F("Firmware ver. "));
HardwareSerial::print((versiondata >> 16) & 0xFF, DEC);
HardwareSerial::print('.');
HardwareSerial::println((versiondata >> 8) & 0xFF, DEC);
```

## Required Fixes

### Fix 1: Update src/rfid_controller.cpp

Replace all occurrences of `HardwareSerial::print` and `HardwareSerial::println` with `Serial.print` and `Serial.println`.

#### Line 23:
**Before:**
```cpp
HardwareSerial::print(F("Didn't find PN53x board"));
```
**After:**
```cpp
Serial.print(F("Didn't find PN53x board"));
```

#### Lines 74-79:
**Before:**
```cpp
HardwareSerial::print(F("Found chip PN5"));
HardwareSerial::println((versiondata >> 24) & 0xFF, HEX);
HardwareSerial::print(F("Firmware ver. "));
HardwareSerial::print((versiondata >> 16) & 0xFF, DEC);
HardwareSerial::print('.');
HardwareSerial::println((versiondata >> 8) & 0xFF, DEC);
```
**After:**
```cpp
Serial.print(F("Found chip PN5"));
Serial.println((versiondata >> 24) & 0xFF, HEX);
Serial.print(F("Firmware ver. "));
Serial.print((versiondata >> 16) & 0xFF, DEC);
Serial.print('.');
Serial.println((versiondata >> 8) & 0xFF, DEC);
```

### Fix 2: Update include/rfid_controller.h

Add a compatibility define to handle the case difference between the mock environment (lowercase `serial`) and production Arduino (uppercase `Serial`).

Add the following lines after the include statements (around line 10):

```cpp
#ifdef UNIT_TEST
    #define Serial serial  // Map uppercase Serial to lowercase serial for tests
#endif
```

## Complete Fixed Code Sections

### src/rfid_controller.cpp - begin() method (lines 18-30):
```cpp
bool RFIDController::begin() {
    m_nfc->begin();

    uint32_t versiondata = m_nfc->getFirmwareVersion();
    if (versiondata == 0u) {
        Serial.print(F("Didn't find PN53x board"));
        return false;
    }

    // Configure board to read RFID tags
    m_nfc->SAMConfig();
    return true;
}
```

### src/rfid_controller.cpp - printFirmwareVersion() method (lines 71-82):
```cpp
void RFIDController::printFirmwareVersion() {
    uint32_t versiondata = getFirmwareVersion();
    if (versiondata != 0u) {
        Serial.print(F("Found chip PN5"));
        Serial.println((versiondata >> 24) & 0xFF, HEX);
        Serial.print(F("Firmware ver. "));
        Serial.print((versiondata >> 16) & 0xFF, DEC);
        Serial.print('.');
        Serial.println((versiondata >> 8) & 0xFF, DEC);
    }
}
```

### include/rfid_controller.h - Updated with Serial mapping (lines 1-12):
```cpp
#pragma once
#include <array>

#ifdef UNIT_TEST
    #include "mock_arduino.h"
    #include "mock_pn532.h"  // Mock PN532 for unit testing
    #define Serial serial  // Map uppercase Serial to lowercase serial for tests
#else
    #include <Adafruit_PN532.h>
    #include <Arduino.h>
#endif
```

## Verification

After applying these fixes:
1. The compilation error should be resolved
2. Serial output will work correctly in both production and test environments
3. The code maintains the same functionality while using the correct Arduino Serial API

## Additional Notes

- In Arduino, `Serial` is a global instance of the HardwareSerial class
- The mock environment uses lowercase `serial` which is why we need the compatibility define
- All Serial communication should use the instance methods, not static class methods
