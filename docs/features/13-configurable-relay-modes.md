# Feature: Configurable Relay Modes

**Complexity**: 🟢 Low
**Hardware Required**: ✅ None
**User Value**: ⭐⭐⭐ Essential

## Overview

Allow configuration of relay behavior including normally open (NO), normally closed (NC), fail-safe/fail-secure modes, and custom timing. This provides flexibility for different installation scenarios and security requirements without hardware changes.

## Benefits

- Adapt to different relay types and wiring
- Choose security posture (fail-safe vs fail-secure)
- Support various door lock mechanisms
- Enable advanced control patterns
- Simplify installation flexibility

## Relay Mode Options

### 1. **Output Polarity**
- **Active HIGH**: HIGH signal = relay ON
- **Active LOW**: LOW signal = relay ON (current default)

### 2. **Default State**
- **Normally OFF**: Relay off when idle
- **Normally ON**: Relay on when idle

### 3. **Fail Behavior**
- **Fail-Safe**: Unlocks on power loss
- **Fail-Secure**: Locks on power loss
- **Fail-Last**: Maintains last state

### 4. **Activation Patterns**
- **Momentary**: Active for set duration
- **Latching**: Stays active until reset
- **Pulsed**: Multiple pulses
- **Toggle**: Changes state each activation

## Implementation Checklist

### Phase 1: Relay Configuration Structure
- [ ] Create configuration system:
  ```cpp
  struct RelayConfig {
      // Basic settings
      bool activeHigh;        // true = HIGH activates, false = LOW
      bool defaultState;      // true = normally on, false = normally off

      // Timing settings
      uint32_t activeDuration;  // ms (0 = latching)
      uint32_t pulseOnTime;     // ms for pulse mode
      uint32_t pulseOffTime;    // ms for pulse mode
      uint8_t pulseCount;       // number of pulses

      // Advanced settings
      enum FailMode {
          FAIL_SAFE,      // Activate on power loss
          FAIL_SECURE,    // Deactivate on power loss
          FAIL_LAST       // Maintain state
      } failMode;

      enum ActivationMode {
          MODE_MOMENTARY,  // Active for duration
          MODE_LATCHING,   // Stay active
          MODE_PULSED,     // Pulse pattern
          MODE_TOGGLE      // Toggle on each trigger
      } mode;
  };
  ```

### Phase 2: Enhanced Relay Controller
- [ ] Modify `RelayController` class:
  ```cpp
  class RelayController {
  private:
      RelayConfig configs[NUM_RELAYS];
      uint32_t activationTime[NUM_RELAYS];
      bool toggleState[NUM_RELAYS];

  public:
      void setRelayConfig(uint8_t relay, const RelayConfig& config);
      RelayConfig getRelayConfig(uint8_t relay);
      void activateRelay(uint8_t relay);  // Uses configured mode
      void updateRelays();  // Call in loop() for timing
      void loadDefaultConfigs();
      void saveConfigs();  // To EEPROM
  };
  ```

### Phase 3: Default Configurations
- [ ] Common presets:
  ```cpp
  // Door Lock (Fail-Safe)
  RelayConfig doorLockFailSafe = {
      .activeHigh = false,
      .defaultState = false,
      .activeDuration = 10000,  // 10 seconds
      .failMode = FAIL_SAFE,
      .mode = MODE_MOMENTARY
  };

  // Door Lock (Fail-Secure)
  RelayConfig doorLockFailSecure = {
      .activeHigh = false,
      .defaultState = true,   // Normally locked
      .activeDuration = 10000,
      .failMode = FAIL_SECURE,
      .mode = MODE_MOMENTARY
  };

  // Alarm Siren
  RelayConfig alarmSiren = {
      .activeHigh = true,
      .defaultState = false,
      .pulseOnTime = 500,
      .pulseOffTime = 500,
      .pulseCount = 10,
      .mode = MODE_PULSED
  };

  // Light Control
  RelayConfig lightControl = {
      .activeHigh = true,
      .defaultState = false,
      .activeDuration = 0,  // Latching
      .mode = MODE_TOGGLE
  };
  ```

### Phase 4: Configuration Interface
- [ ] Serial commands:
  ```
  RELAY_CONFIG <relay> <param> <value>
  RELAY_PRESET <relay> <preset_name>
  RELAY_TEST <relay>
  RELAY_STATUS
  SAVE_RELAY_CONFIG
  ```

- [ ] Example commands:
  ```
  RELAY_CONFIG 1 ACTIVE_HIGH true
  RELAY_CONFIG 1 DURATION 5000
  RELAY_PRESET 2 ALARM_SIREN
  RELAY_TEST 1
  ```

### Phase 5: EEPROM Storage
- [ ] Save configurations:
  ```cpp
  #define RELAY_CONFIG_ADDR 100  // EEPROM address

  void saveRelayConfigs() {
      for (int i = 0; i < NUM_RELAYS; i++) {
          int addr = RELAY_CONFIG_ADDR + (i * sizeof(RelayConfig));
          EEPROM.put(addr, configs[i]);
      }
  }

  void loadRelayConfigs() {
      for (int i = 0; i < NUM_RELAYS; i++) {
          int addr = RELAY_CONFIG_ADDR + (i * sizeof(RelayConfig));
          EEPROM.get(addr, configs[i]);

          // Validate loaded config
          if (configs[i].activeDuration > 3600000) {  // > 1 hour
              loadDefaultConfig(i);  // Load safe default
          }
      }
  }
  ```

### Phase 6: Advanced Features
- [ ] **Conditional Activation**:
  ```cpp
  struct ConditionalRelay {
      uint8_t dependsOn;      // Another relay number
      bool requiresState;     // Required state of dependency
      uint32_t delay;         // Delay after condition met
  };
  ```

- [ ] **Sequenced Activation**:
  ```cpp
  // Activate relays in sequence
  void activateSequence(uint8_t relays[], uint8_t count, uint32_t delay) {
      for (int i = 0; i < count; i++) {
          activateRelay(relays[i]);
          delay(delay);
      }
  }
  ```

- [ ] **Grouped Control**:
  ```cpp
  // Group relays for simultaneous control
  struct RelayGroup {
      char name[16];
      uint8_t relays[NUM_RELAYS];
      uint8_t count;
  };
  ```

### Phase 7: Safety Features
- [ ] **Overcurrent Protection**:
  - Monitor relay current if possible
  - Auto-shutoff on overcurrent
  - Cooldown period

- [ ] **Mutual Exclusion**:
  - Prevent conflicting relays from activating
  - Example: Up/Down motor controls

- [ ] **Maximum On-Time**:
  - Force deactivation after max time
  - Prevent stuck relays

## Usage Examples

### Example 1: Fail-Safe Door Lock
```cpp
// Door unlocks if power is lost
RelayConfig doorConfig;
doorConfig.activeHigh = false;  // Active LOW relay
doorConfig.defaultState = false; // Normally locked
doorConfig.failMode = FAIL_SAFE;
doorConfig.mode = MODE_MOMENTARY;
doorConfig.activeDuration = 10000; // 10 seconds
relays.setRelayConfig(0, doorConfig);
```

### Example 2: Emergency Strobe Light
```cpp
// Flashing pattern for emergencies
RelayConfig strobeConfig;
strobeConfig.activeHigh = true;
strobeConfig.mode = MODE_PULSED;
strobeConfig.pulseOnTime = 200;   // 200ms on
strobeConfig.pulseOffTime = 200;  // 200ms off
strobeConfig.pulseCount = 0;      // Continuous
relays.setRelayConfig(2, strobeConfig);
```

### Example 3: Toggle Light Switch
```cpp
// Each card scan toggles the light
RelayConfig lightConfig;
lightConfig.mode = MODE_TOGGLE;
lightConfig.activeHigh = true;
relays.setRelayConfig(3, lightConfig);
```

## Testing Checklist

- [ ] Test all relay modes
- [ ] Verify polarity settings
- [ ] Test timing accuracy
- [ ] Verify EEPROM persistence
- [ ] Test fail modes (simulate power loss)
- [ ] Verify serial configuration
- [ ] Test preset applications
- [ ] Validate safety limits

## Configuration Storage

EEPROM Memory Map:
```
Address   Content
0-99      Reserved for UIDs
100-115   Relay 1 Config
116-131   Relay 2 Config
132-147   Relay 3 Config
148-163   Relay 4 Config
164-199   Reserved
200+      Future use
```

## Future Enhancements

- Web-based configuration interface
- Relay state feedback (current sensing)
- Smart home integration presets
- Machine learning for usage patterns
- Predictive maintenance alerts
- Cloud backup of configurations
