# Feature: Multiple Access Levels

**Complexity**: 🟡 Medium
**Hardware Required**: ✅ None
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement different access levels for different RFID cards, allowing for role-based access control. For example, owner cards could have full access, while service cards might have time-limited or feature-limited access.

## Benefits

- Enhanced security with granular permissions
- Different relay activations for different users
- Temporary access for guests or service personnel
- Admin cards for system configuration
- Audit trail possibilities with user identification

## Access Level Types

1. **Owner/Admin** - Full system access, can program new cards
2. **Regular User** - Standard access, activates primary relay
3. **Service** - Time-limited access, specific relay activation
4. **Guest** - Temporary access with expiration
5. **Emergency** - Override card for first responders

## Implementation Checklist

### Phase 1: Data Structure Updates
- [ ] Modify `RFIDController` to support access levels:
  ```cpp
  struct AuthorizedCard {
      uint8_t uid[7];
      uint8_t uidLength;
      uint8_t accessLevel;
      uint32_t expirationTime; // Unix timestamp, 0 = no expiration
      uint8_t allowedRelays; // Bitmask for relay permissions
  };
  ```
- [ ] Update UID storage arrays to include metadata
- [ ] Create access level constants:
  ```cpp
  enum AccessLevel {
      ACCESS_DENIED = 0,
      ACCESS_GUEST = 1,
      ACCESS_SERVICE = 2,
      ACCESS_USER = 3,
      ACCESS_ADMIN = 4,
      ACCESS_EMERGENCY = 5
  };
  ```

### Phase 2: Core Functionality
- [ ] Modify `validateUID()` to return access level instead of boolean
- [ ] Implement `getCardPermissions()` method
- [ ] Add relay permission checking:
  - [ ] `bool canActivateRelay(uint8_t accessLevel, uint8_t relayNum)`
  - [ ] Relay bitmask validation
- [ ] Implement card expiration checking
- [ ] Add method to check time-based restrictions

### Phase 3: Access Level Behaviors
- [ ] **Admin Level**:
  - [ ] Can activate all relays
  - [ ] Extended door unlock time (30 seconds)
  - [ ] Can enter programming mode
  - [ ] No time restrictions

- [ ] **User Level**:
  - [ ] Standard relay activation (Relay 1)
  - [ ] Normal unlock time (10 seconds)
  - [ ] Optional time-of-day restrictions

- [ ] **Service Level**:
  - [ ] Specific relay activation (e.g., Relay 2 for hood release)
  - [ ] Time window restrictions (e.g., 8 AM - 5 PM)
  - [ ] Day-of-week restrictions
  - [ ] Access logging required

- [ ] **Guest Level**:
  - [ ] Limited relay access
  - [ ] Card expiration date checking
  - [ ] Reduced unlock time (5 seconds)
  - [ ] Access count limitations

- [ ] **Emergency Level**:
  - [ ] All relays activate immediately
  - [ ] Bypass all time restrictions
  - [ ] Trigger emergency notification
  - [ ] Log with high priority

### Phase 4: Time-Based Features
- [ ] Add RTC (Real Time Clock) support:
  - [ ] DS3231 RTC module integration
  - [ ] Time synchronization methods
  - [ ] Battery backup for timekeeping
- [ ] Implement schedule checking:
  ```cpp
  struct AccessSchedule {
      uint8_t allowedDays; // Bitmask for days of week
      uint8_t startHour;
      uint8_t startMinute;
      uint8_t endHour;
      uint8_t endMinute;
  };
  ```
- [ ] Holiday/blackout date support

### Phase 5: Enhanced Audio/LED Feedback
- [ ] Different audio tracks for different access levels
- [ ] LED patterns based on access level:
  - [ ] Admin: Purple fade
  - [ ] User: Green blink
  - [ ] Service: Yellow pattern
  - [ ] Guest: Blue flash
  - [ ] Emergency: Red strobe
- [ ] Access level announcement option

### Phase 6: Configuration Interface
- [ ] Serial command interface for card management:
  - [ ] ADD_CARD <UID> <LEVEL> <EXPIRATION>
  - [ ] REMOVE_CARD <UID>
  - [ ] LIST_CARDS
  - [ ] SET_SCHEDULE <UID> <SCHEDULE>
- [ ] Card cloning prevention
- [ ] Backup/restore functionality

## Example Usage Scenarios

1. **Owner Access**: Full access anytime, all features
2. **Family Member**: Normal access, restricted late at night
3. **Mechanic**: Hood release only, weekdays 8-5
4. **Dog Walker**: Door access only, expires in 30 days
5. **Emergency**: Fire department override card

## Testing Checklist

- [ ] Test each access level individually
- [ ] Verify relay permissions work correctly
- [ ] Test time-based restrictions
- [ ] Verify card expiration
- [ ] Test emergency override
- [ ] Verify different audio/visual feedback
- [ ] Test serial configuration commands
- [ ] Stress test with multiple card reads

## Security Considerations

- [ ] Implement card blacklist for lost/stolen cards
- [ ] Add anti-passback feature (prevent card sharing)
- [ ] Encrypted card data storage
- [ ] Tamper detection and response
- [ ] Failed attempt tracking per card

## Future Enhancements

- Two-factor authentication (card + PIN)
- Biometric integration (fingerprint)
- Mobile app card provisioning
- Cloud-based access management
- Integration with home automation systems
