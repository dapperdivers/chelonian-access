# Feature: Emergency Override

**Complexity**: 🟢 Low
**Hardware Required**: 🔧 Minor (Physical button or none)
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement emergency override mechanisms for situations where normal access methods fail or emergency responders need immediate entry. This includes physical buttons, special emergency cards, or remote triggers.

## Benefits

- Life safety compliance
- Emergency responder access
- Failsafe during system failures
- Panic button functionality
- Compliance with fire codes

## Override Methods

1. **Physical Button** - Direct relay activation
2. **Emergency Card** - Special RFID card with override
3. **Break Glass** - Traditional emergency release
4. **Remote Trigger** - Network/phone activation
5. **Power Failure** - Fail-safe or fail-secure options

## Implementation Checklist

### Phase 1: Physical Button Override
- [ ] Hardware setup:
  ```cpp
  class EmergencyOverride {
  private:
      uint8_t buttonPin;
      uint8_t glassBreakPin;
      bool overrideActive;
      millis_t overrideTime;
  public:
      void begin();
      bool checkOverride();
      void activateOverride(uint8_t source);
      void resetOverride();
  };
  ```
- [ ] Button types:
  - [ ] Momentary push button
  - [ ] Key switch
  - [ ] Mushroom emergency stop
  - [ ] Recessed button (prevent accidental activation)

### Phase 2: Emergency Card System
- [ ] Special card designation:
  ```cpp
  struct EmergencyCard {
      uint8_t uid[7];
      uint8_t uidLength;
      uint8_t overrideLevel;  // Fire, Police, Medical, Master
      bool logOnly;           // For audit without unlock
      uint32_t validUntil;    // Expiration date
  };
  ```
- [ ] Override behaviors:
  - [ ] Instant all-relay activation
  - [ ] Extended unlock duration
  - [ ] Disable normal access temporarily
  - [ ] Trigger alarms/notifications

### Phase 3: Override Logic
- [ ] **Activation Triggers**:
  - [ ] Physical button press
  - [ ] Emergency card scan
  - [ ] Glass break sensor
  - [ ] Remote command
  - [ ] Power failure (configurable)

- [ ] **System Response**:
  ```cpp
  void handleEmergencyOverride() {
      // 1. Activate all door relays
      relays.setAllRelays(true);

      // 2. Disable normal card reading
      emergencyMode = true;

      // 3. Log emergency event
      logger.logEmergency(source, timestamp);

      // 4. Sound alarm (if configured)
      audio.playTrack(SOUND_EMERGENCY);

      // 5. Send notifications
      remote.sendAlert("EMERGENCY OVERRIDE ACTIVATED");
  }
  ```

### Phase 4: Safety Features
- [ ] **Fail-Safe vs Fail-Secure**:
  ```cpp
  enum PowerFailureMode {
      FAIL_SAFE,    // Doors unlock on power loss
      FAIL_SECURE,  // Doors remain locked
      BATTERY_BACKUP // Maintain normal operation
  };
  ```
- [ ] Capacitor backup for relay activation
- [ ] Battery monitoring and alerts
- [ ] Graceful degradation

### Phase 5: Button Installation
- [ ] **Indoor Emergency Release**:
  - [ ] Green button near exit
  - [ ] "Push to Exit" signage
  - [ ] LED illumination
  - [ ] ADA compliant height

- [ ] **Outdoor Emergency Access**:
  - [ ] Knox box integration
  - [ ] Weatherproof enclosure
  - [ ] Tamper detection
  - [ ] Key or code protection

### Phase 6: Notification System
- [ ] **Local Alerts**:
  - [ ] Loud alarm/siren
  - [ ] Strobe light activation
  - [ ] LED status indicators
  - [ ] Voice announcement

- [ ] **Remote Alerts**:
  - [ ] SMS/Email notifications
  - [ ] Security system integration
  - [ ] Building management alert
  - [ ] Emergency services notification

### Phase 7: Reset and Recovery
- [ ] **Manual Reset**:
  ```cpp
  void resetEmergencyMode() {
      // 1. Verify authorized reset
      if (!checkResetAuthorization()) return;

      // 2. Return to normal operation
      emergencyMode = false;

      // 3. Re-lock doors (with delay)
      delay(EXIT_DELAY);
      relays.setAllRelays(false);

      // 4. Log reset event
      logger.logReset(resetUser, timestamp);

      // 5. System check
      performSystemCheck();
  }
  ```
- [ ] Reset authorization methods
- [ ] Automatic timeout options
- [ ] Post-emergency audit

## Wiring Examples

### Emergency Button Circuit
```
Arduino Pin 2 ----+---- 10K Resistor ---- 3.3V
                  |
                  +---- Emergency Button ---- GND
```

### Break Glass Sensor
```
Arduino Pin 3 ----+---- 10K Resistor ---- 3.3V
                  |
                  +---- Glass Break NC Contact ---- GND
```

## Compliance Considerations

- [ ] **Fire Code Requirements**:
  - [ ] Clearly marked emergency exit
  - [ ] Illuminated exit signs
  - [ ] Panic hardware on doors
  - [ ] Regular testing requirements

- [ ] **ADA Compliance**:
  - [ ] Button height 48" max
  - [ ] 5 lbs max activation force
  - [ ] Clear floor space
  - [ ] Visual and audible indicators

## Testing Checklist

- [ ] Test all override methods
- [ ] Verify immediate relay response
- [ ] Test notification systems
- [ ] Verify logging accuracy
- [ ] Test reset procedures
- [ ] Power failure testing
- [ ] Integration with fire alarm
- [ ] Monthly test schedule

## Configuration Options

```cpp
// Emergency configuration
#define EMERGENCY_UNLOCK_ALL true      // Unlock all relays
#define EMERGENCY_DURATION 300000      // 5 minutes
#define REQUIRE_RESET true             // Manual reset needed
#define ALERT_VOLUME 30                // Maximum volume
#define POWER_FAIL_MODE FAIL_SAFE      // Fire code compliance
#define LOG_EMERGENCY_CARDS true       // Track responder access
```

## Future Enhancements

- Integration with fire alarm system
- Smartphone emergency unlock
- Biometric override (first responder fingerprints)
- GPS-based emergency unlock
- Voice-activated emergency release
- Automatic emergency services dispatch
