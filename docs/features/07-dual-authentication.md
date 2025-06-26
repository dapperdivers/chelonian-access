# Feature: Dual Authentication

**Complexity**: 🟡 Medium
**Hardware Required**: 🔧 Minor (Keypad or second RFID reader)
**User Value**: ⭐⭐ Nice-to-have

## Overview

Implement two-factor authentication requiring two different credentials for high-security access. This could be two cards simultaneously, card plus PIN, or card plus biometric.

## Benefits

- Significantly enhanced security
- Prevents single card theft vulnerability
- Compliance with high-security standards
- Flexible authentication methods
- Audit trail for both factors

## Authentication Combinations

1. **Dual Card** - Two different RFID cards
2. **Card + PIN** - RFID card plus keypad code
3. **Card + Biometric** - RFID plus fingerprint
4. **Time-separated Cards** - Two cards within time window
5. **Card + Mobile** - RFID plus smartphone confirmation

## Implementation Checklist

### Phase 1: Dual Card System
- [ ] Hardware setup for two RFID readers:
  ```cpp
  class DualRFIDController {
  private:
      RFIDController reader1;
      RFIDController reader2;
      bool card1Valid;
      bool card2Valid;
      millis_t card1Time;
      millis_t card2Time;
  public:
      bool checkDualAuth();
      void setTimeout(uint16_t seconds);
  };
  ```
- [ ] Simultaneous card detection
- [ ] Time window for second card (e.g., 10 seconds)
- [ ] Different cards requirement

### Phase 2: Card + PIN System
- [ ] Keypad hardware integration:
  - [ ] 4x3 or 4x4 matrix keypad
  - [ ] I2C keypad for fewer pins
  - [ ] Weatherproof option
- [ ] PIN management:
  ```cpp
  struct UserPIN {
      uint8_t uid[7];
      uint8_t uidLength;
      uint16_t pin;  // 4-6 digit PIN
      uint8_t attempts;
      millis_t lockoutTime;
  };
  ```
- [ ] PIN entry flow:
  - [ ] Scan card first
  - [ ] Prompt for PIN (LED/audio)
  - [ ] Timeout handling
  - [ ] Wrong PIN lockout

### Phase 3: Authentication Logic
- [ ] **State Machine**:
  ```cpp
  enum AuthState {
      AUTH_IDLE,
      AUTH_CARD1_VALID,
      AUTH_WAITING_SECOND,
      AUTH_BOTH_VALID,
      AUTH_FAILED,
      AUTH_LOCKOUT
  };
  ```
- [ ] **Validation Rules**:
  - [ ] Both factors within time window
  - [ ] Different cards for dual-card
  - [ ] Correct PIN for card+PIN
  - [ ] Anti-replay protection

### Phase 4: Configuration Options
- [ ] **Selective Dual Auth**:
  ```cpp
  struct AuthRequirements {
      uint8_t relayNumber;
      bool requireDualAuth;
      uint8_t authMethod;  // DUAL_CARD, CARD_PIN, etc.
      uint16_t timeWindow; // seconds
  };
  ```
- [ ] Time-based requirements:
  - [ ] Single auth during business hours
  - [ ] Dual auth after hours
  - [ ] Weekend dual auth
- [ ] Access level based:
  - [ ] Admin always dual auth
  - [ ] Service single auth
  - [ ] High-security areas dual auth

### Phase 5: User Interface
- [ ] **LED Indicators**:
  ```cpp
  // Authentication progress
  FIRST_FACTOR_OK:  Green pulse
  AWAITING_SECOND:  Yellow pulse
  DUAL_AUTH_OK:     Solid green
  AUTH_FAILED:      Red flash
  TIMEOUT:          Orange fade
  ```
- [ ] **Audio Feedback**:
  - [ ] First factor accepted tone
  - [ ] Awaiting second factor prompt
  - [ ] Success melody
  - [ ] Failure buzzer
  - [ ] Timeout warning

### Phase 6: Enhanced Security
- [ ] **Anti-Collusion**:
  - [ ] Prevent same person with two cards
  - [ ] Minimum time between factors
  - [ ] Location-based validation (two readers apart)

- [ ] **Buddy System**:
  - [ ] Designated card pairs
  - [ ] Supervisor + employee combinations
  - [ ] Rotation schedules

- [ ] **Emergency Override**:
  - [ ] Single card override in emergencies
  - [ ] Logged with high priority
  - [ ] Notification to security

### Phase 7: PIN Management
- [ ] **PIN Features**:
  - [ ] User-changeable PINs
  - [ ] PIN complexity rules
  - [ ] Expiration and rotation
  - [ ] Duress PIN (silent alarm)

- [ ] **Storage**:
  ```cpp
  // Secure PIN storage
  - Hash PINs with salt
  - Store in EEPROM
  - Encrypted if possible
  - Regular backup
  ```

## Hardware Options

### Keypad Integration
```cpp
// 4x4 Keypad connection
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
```

### Dual RFID Setup
- Second PN532 module on different SS pin
- Separate antennas at least 10cm apart
- Shield between readers to prevent interference

## Testing Checklist

- [ ] Test all authentication methods
- [ ] Verify timeout handling
- [ ] Test lockout mechanisms
- [ ] Verify anti-replay protection
- [ ] Test emergency override
- [ ] Check PIN security
- [ ] Verify logging of both factors
- [ ] Test hardware reliability

## Use Cases

1. **Server Room**: IT admin card + personal PIN
2. **Safe Access**: Manager + assistant cards simultaneously
3. **Chemical Storage**: Safety officer card + daily rotating PIN
4. **Night Access**: Security guard card + supervisor approval
5. **Clean Room**: Badge + biometric verification

## Future Enhancements

- Biometric integration (fingerprint/face)
- Mobile phone as second factor
- Time-based OTP (like Google Authenticator)
- Bluetooth proximity detection
- Voice recognition
- Behavioral authentication patterns
