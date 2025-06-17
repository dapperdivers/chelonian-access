# Feature: Scheduled Relay Control

**Complexity**: 🟡 Medium
**Hardware Required**: 🔧 Minor (RTC module)
**User Value**: ⭐⭐⭐ Essential

## Overview

Enable time-based scheduling for relay activation and deactivation, allowing relays to turn on/off at specific times of day, days of week, or date ranges. Perfect for lighting control, HVAC systems, or any automation that follows a schedule.

## Benefits

- Automatic lighting control
- Energy savings through scheduled operation
- Business hours automation
- Seasonal adjustments
- No manual intervention needed
- Override capability for holidays

## Schedule Types

1. **Daily Schedules** - Same time every day
2. **Weekly Schedules** - Different times for different days
3. **Date Range** - Active only during specific dates
4. **Recurring Events** - Monthly, yearly patterns
5. **Exception Days** - Holidays, special events

## Implementation Checklist

### Phase 1: Schedule Data Structure
- [ ] Define schedule system:
  ```cpp
  struct RelaySchedule {
      bool enabled;
      uint8_t relayNumber;

      // Time windows (up to 4 per relay)
      struct TimeWindow {
          uint8_t startHour;
          uint8_t startMinute;
          uint8_t endHour;
          uint8_t endMinute;
          uint8_t daysOfWeek;  // Bitmask: Sun=1, Mon=2, etc.
          bool active;
      } windows[4];

      // Date restrictions
      uint32_t validFrom;    // Unix timestamp (0 = no restriction)
      uint32_t validUntil;   // Unix timestamp (0 = no restriction)

      // Override settings
      bool overrideActive;
      bool overrideState;    // ON/OFF during override
  };
  ```

### Phase 2: Schedule Controller
- [ ] Create `ScheduleController` class:
  ```cpp
  class ScheduleController {
  private:
      RelaySchedule schedules[NUM_RELAYS];
      DateTime lastCheck;
      RelayController* relayCtrl;
      TimeController* timeCtrl;

  public:
      bool begin(RelayController* rc, TimeController* tc);
      void checkSchedules();  // Call frequently in loop()

      // Schedule management
      void setSchedule(uint8_t relay, const RelaySchedule& schedule);
      RelaySchedule getSchedule(uint8_t relay);
      void enableSchedule(uint8_t relay, bool enable);

      // Time window management
      void addTimeWindow(uint8_t relay, uint8_t window,
                        uint8_t startH, uint8_t startM,
                        uint8_t endH, uint8_t endM,
                        uint8_t days);
      void removeTimeWindow(uint8_t relay, uint8_t window);

      // Override functions
      void setOverride(uint8_t relay, bool state, uint32_t duration);
      void clearOverride(uint8_t relay);
      void clearAllOverrides();

      // Persistence
      void saveSchedules();
      void loadSchedules();
  };
  ```

### Phase 3: Common Use Cases
- [ ] Preset schedules:
  ```cpp
  // Business Hours Lighting
  RelaySchedule businessLighting = {
      .enabled = true,
      .relayNumber = 2,
      .windows = {
          {8, 0, 18, 0, 0b01111100, true},  // M-F 8AM-6PM
          {10, 0, 14, 0, 0b01000000, true}, // Sat 10AM-2PM
      }
  };

  // Security Lighting (Dusk to Dawn)
  RelaySchedule securityLighting = {
      .enabled = true,
      .relayNumber = 3,
      .windows = {
          {18, 30, 23, 59, 0b11111111, true}, // 6:30PM-Midnight
          {0, 0, 6, 30, 0b11111111, true},    // Midnight-6:30AM
      }
  };

  // HVAC Schedule
  RelaySchedule hvacSchedule = {
      .enabled = true,
      .relayNumber = 4,
      .windows = {
          {7, 0, 9, 0, 0b01111100, true},   // M-F Morning
          {17, 0, 22, 0, 0b01111100, true}, // M-F Evening
          {8, 0, 22, 0, 0b11000011, true},  // Weekends all day
      }
  };
  ```

### Phase 4: Advanced Scheduling
- [ ] **Sunrise/Sunset Calculation**:
  ```cpp
  struct LocationConfig {
      float latitude;
      float longitude;
      int8_t timezone;
  };

  void calculateSunriseSunset(DateTime date,
                              float& sunrise,
                              float& sunset);

  // Automatic adjustment for outdoor lighting
  void adjustForDaylight(RelaySchedule& schedule);
  ```

- [ ] **Holiday Management**:
  ```cpp
  struct Holiday {
      uint8_t month;
      uint8_t day;
      char name[16];
      bool skipSchedule;  // Skip normal schedule
      RelaySchedule override;  // Special schedule
  };

  Holiday holidays[20];  // Store up to 20 holidays
  ```

- [ ] **Learning Mode**:
  ```cpp
  // Learn patterns from manual operation
  void recordManualOperation(uint8_t relay, bool state);
  void suggestSchedule(uint8_t relay);
  ```

### Phase 5: User Interface
- [ ] Serial commands:
  ```
  SCHEDULE SET <relay> <window> <start> <end> <days>
  SCHEDULE ENABLE <relay> <true/false>
  SCHEDULE OVERRIDE <relay> <ON/OFF> <minutes>
  SCHEDULE CLEAR <relay>
  SCHEDULE LIST
  SCHEDULE HOLIDAY <date> <name> <relay> <action>
  TIME SET <YYYY-MM-DD HH:MM:SS>
  ```

- [ ] Example commands:
  ```
  SCHEDULE SET 2 1 08:00 18:00 MTWTF    // Relay 2 on M-F 8AM-6PM
  SCHEDULE SET 3 1 SUNSET SUNRISE ALL   // Relay 3 sunset to sunrise
  SCHEDULE OVERRIDE 1 ON 60             // Override relay 1 ON for 60 min
  SCHEDULE HOLIDAY 2024-12-25 Christmas 2 OFF  // Relay 2 off on Christmas
  ```

### Phase 6: Integration Features
- [ ] **Cascade Control**:
  ```cpp
  // One relay schedule triggers another
  struct CascadeRule {
      uint8_t sourceRelay;
      uint8_t targetRelay;
      int8_t delayMinutes;  // + for after, - for before
      bool invertState;     // Target opposite of source
  };
  ```

- [ ] **Conditional Scheduling**:
  ```cpp
  // Schedule based on sensor input
  struct ConditionalSchedule {
      uint8_t sensorPin;
      int16_t threshold;
      bool aboveThreshold;  // Activate when above/below
      RelaySchedule schedule;
  };
  ```

### Phase 7: Visual Feedback
- [ ] LED indicators for schedule status:
  - Solid: Schedule active, relay on
  - Slow blink: Schedule active, relay off
  - Fast blink: Override active
  - Off: Schedule disabled

- [ ] Audio notifications:
  - Chime when schedule activates
  - Different tones for on/off
  - Alert for override activation

## Memory Management

### EEPROM Layout
```
Address    Content
200-299    Relay 1 Schedules
300-399    Relay 2 Schedules
400-499    Relay 3 Schedules
500-599    Relay 4 Schedules
600-649    Holidays
650-699    Location config
700+       Reserved
```

### SD Card Storage (if available)
```
/SCHEDULES/
├── relay1.json
├── relay2.json
├── relay3.json
├── relay4.json
├── holidays.json
└── history.log
```

## Testing Checklist

- [ ] Basic schedule on/off
- [ ] Multi-window schedules
- [ ] Day of week restrictions
- [ ] Date range validation
- [ ] Override functionality
- [ ] Holiday handling
- [ ] RTC accuracy
- [ ] Power loss recovery
- [ ] Daylight saving time

## Example Applications

1. **Parking Lot Lights**: On at sunset, off at sunrise
2. **Office HVAC**: On at 7 AM, off at 6 PM weekdays
3. **Store Sign**: On during business hours + 1 hour
4. **Greenhouse**: Multiple windows for different seasons
5. **Pool Pump**: Run 4 hours in morning, 4 in evening

## Future Enhancements

- Web interface for schedule management
- Mobile app with calendar view
- Integration with weather data
- Astronomical calculations (moon phases)
- Energy usage tracking and optimization
- Machine learning for optimal scheduling
