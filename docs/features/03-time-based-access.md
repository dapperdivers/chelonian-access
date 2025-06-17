# Feature: Time-Based Access Control

**Complexity**: 🟡 Medium
**Hardware Required**: 🔧 Minor (RTC module)
**User Value**: ⭐⭐ Nice-to-have

## Overview

Implement time-based restrictions for access control, allowing cards to work only during specific hours, days, or date ranges. This requires adding a Real-Time Clock (RTC) module like the DS3231.

## Benefits

- Restrict access to business hours
- Temporary guest access with auto-expiration
- Different schedules for different users
- Holiday and weekend restrictions
- Automatic lockdown during off-hours

## Hardware Requirements

- DS3231 RTC Module (~$5)
- I2C connection (SDA/SCL pins)
- CR2032 battery for RTC backup

## Implementation Checklist

### Phase 1: RTC Integration
- [ ] Add DS3231 library to `platformio.ini`
- [ ] Create `TimeController` class:
  ```cpp
  class TimeController {
  public:
      bool begin();
      DateTime now();
      void setTime(DateTime dt);
      bool isTimeValid();
      void syncFromSerial();
  };
  ```
- [ ] Implement I2C communication with RTC
- [ ] Add time synchronization on startup
- [ ] Battery backup detection

### Phase 2: Time Validation Logic
- [ ] Create time-based structures:
  ```cpp
  struct TimeWindow {
      uint8_t startHour;
      uint8_t startMinute;
      uint8_t endHour;
      uint8_t endMinute;
  };

  struct DateRange {
      uint32_t startDate; // Unix timestamp
      uint32_t endDate;   // Unix timestamp
  };
  ```
- [ ] Implement validation methods:
  - [ ] `bool isWithinTimeWindow(TimeWindow window)`
  - [ ] `bool isWithinDateRange(DateRange range)`
  - [ ] `bool isDayAllowed(uint8_t dayMask)`
  - [ ] `bool isHoliday(DateTime date)`

### Phase 3: Card Time Restrictions
- [ ] Extend card data structure:
  ```cpp
  struct TimedAccessCard {
      uint8_t uid[7];
      uint8_t uidLength;
      TimeWindow dailyWindow;
      uint8_t allowedDays;    // Bitmask: Sun=1, Mon=2, etc.
      DateRange validPeriod;
      bool hasTimeRestriction;
  };
  ```
- [ ] Modify access validation to check time
- [ ] Add grace period option (e.g., 5 minutes after hours)

### Phase 4: Schedule Types
- [ ] **Business Hours**:
  - [ ] Monday-Friday, 8 AM - 6 PM
  - [ ] Exclude weekends
  - [ ] Holiday calendar support

- [ ] **Shift-Based**:
  - [ ] Morning shift: 6 AM - 2 PM
  - [ ] Evening shift: 2 PM - 10 PM
  - [ ] Night shift: 10 PM - 6 AM

- [ ] **Custom Schedules**:
  - [ ] Per-card time windows
  - [ ] Multiple time windows per day
  - [ ] Exception dates

### Phase 5: User Interface
- [ ] Serial commands for time management:
  - [ ] `SET_TIME <YYYY-MM-DD HH:MM:SS>`
  - [ ] `GET_TIME`
  - [ ] `SET_SCHEDULE <UID> <SCHEDULE>`
  - [ ] `ADD_HOLIDAY <DATE>`
  - [ ] `LIST_SCHEDULES`
- [ ] Time display on serial monitor
- [ ] Schedule status reporting

### Phase 6: Enhanced Features
- [ ] **Daylight Saving Time**:
  - [ ] Automatic DST adjustment
  - [ ] Timezone configuration

- [ ] **One-Time Access**:
  - [ ] Single-use time windows
  - [ ] Auto-expire after use

- [ ] **Recurring Events**:
  - [ ] Weekly cleaning crew access
  - [ ] Monthly maintenance windows

- [ ] **Early Warning**:
  - [ ] LED/audio warning 5 minutes before lockout
  - [ ] Grace period for active sessions

## Configuration Examples

```cpp
// Office worker - weekdays only
TimeWindow officeHours = {8, 0, 17, 30}; // 8:00 AM - 5:30 PM
uint8_t weekdays = 0b01111100; // Mon-Fri

// Security guard - night shift
TimeWindow nightShift = {22, 0, 6, 0}; // 10:00 PM - 6:00 AM
uint8_t everyday = 0b11111111; // All days

// Contractor - specific project period
DateRange projectPeriod = {1704067200, 1706659200}; // Jan 1 - Jan 31, 2024
```

## Testing Checklist

- [ ] Test RTC accuracy over 24 hours
- [ ] Verify battery backup functionality
- [ ] Test all schedule types
- [ ] Verify timezone handling
- [ ] Test boundary conditions (midnight, etc.)
- [ ] Verify holiday restrictions
- [ ] Test grace period functionality
- [ ] Simulate clock drift and recovery

## Troubleshooting

- [ ] RTC not keeping time: Check battery
- [ ] I2C communication errors: Verify pull-up resistors
- [ ] Time drift: Consider periodic NTP sync (if network available)
- [ ] Timezone issues: Store all times in UTC

## Future Enhancements

- Network Time Protocol (NTP) sync
- Web interface for schedule management
- Calendar integration (Google Calendar, etc.)
- Automatic holiday calendar updates
- Geofencing with GPS time validation
