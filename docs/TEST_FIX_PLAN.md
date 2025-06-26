# Unit Test Fix Plan for Chelonian Access

## Overview
This document outlines the comprehensive plan to fix all unit tests to properly use the mock implementations.

## Phase 1: Test Infrastructure

### 1.1 Fix test_helpers.h (lib/test/)
- Keep the current fixture definitions
- Add missing IntegrationTestFixture
- Ensure proper extern declarations

### 1.2 Fix test_helpers.cpp (test/)
- Remove duplicate fixture definitions
- Use extern fixtures from header
- Add integration fixture support
- Keep helper functions for timing and pin state verification

## Phase 2: Individual Test Files

### 2.1 Audio Controller Tests
**File**: `test/test_native/test_audio/test_audio_controller.cpp`

Issues:
- Duplicate audioFixture definition
- Not using mock methods properly

Fixes:
- Remove local audioFixture definition
- Use global fixture from test_helpers.h
- Access mock methods through fixture->mock pointer
- Update assertions to check mock state

### 2.2 Relay Controller Tests
**File**: `test/test_native/test_relay/test_relay_controller.cpp`

Issues:
- Testing actual hardware pins instead of mock
- Not using IRelayController interface

Fixes:
- Create RelayController instance with mock pins
- Test through IRelayController interface
- Use mock's tracking variables (lastPin, lastState, relayHistory)
- Update all assertions to check mock state

### 2.3 RFID Controller Tests
**File**: `test/test_native/test_rfid/test_rfid_controller.cpp`

Issues:
- Not using MockRFIDController properly
- Creating actual RFIDController instances

Fixes:
- Use rfidFixture from test_helpers
- Set up mock expectations using addMockUID
- Test through IRFIDController interface
- Use mock's validateUIDResult for testing

### 2.4 Access Service Tests
**File**: `test/test_native/test_access/test_access_service.cpp`

Issues:
- Undefined variables (invalidAttempts, relayActive, etc.)
- Not using mock controllers

Fixes:
- Create AccessService instance with all three mocks
- Access state through the service or mocks
- Define missing functions or use service methods
- Test service behavior through its public interface

### 2.5 Integration Tests
**File**: `test/test_native/test_integration/test_integration.cpp`

Issues:
- Missing integrationFixture
- Direct hardware manipulation
- Undefined state structure

Fixes:
- Create IntegrationTestFixture with all mocks and AccessService
- Test complete workflows using mocks
- Track state through mock controllers
- Remove direct pin manipulation

## Phase 3: Implementation Details

### 3.1 New IntegrationTestFixture Structure
```cpp
class IntegrationTestFixture {
public:
    MockRFIDController* mockRFID;
    MockRelayController* mockRelays;
    MockAudioController* mockAudio;
    AccessService* service;

    IntegrationTestFixture() {
        mockRFID = new MockRFIDController();
        mockRelays = new MockRelayController();
        mockAudio = new MockAudioController();
        service = new AccessService(*mockRFID, *mockRelays, *mockAudio);
    }

    ~IntegrationTestFixture() {
        delete service;
        delete mockAudio;
        delete mockRelays;
        delete mockRFID;
    }
};
```

### 3.2 Mock Usage Patterns

#### Audio Mock Usage:
```cpp
// Instead of: audioFixture->audio->playTrack(track)
// The mock automatically tracks: mockAudio->getLastPlayedTrack()

// Instead of: checking hardware
// Check mock state: mockAudio->getVolume(), mockAudio->getStatus()
```

#### Relay Mock Usage:
```cpp
// Instead of: checking mockPinStates[9]
// Check mock state: mockRelays->lastPin, mockRelays->lastState

// Track history: mockRelays->relayHistory
```

#### RFID Mock Usage:
```cpp
// Set up card: mockRFID->addMockUID({0x01, 0x02, 0x03, 0x04})
// Control validation: mockRFID->validateUIDResult = true/false
// Check calls: mockRFID->lastValidatedUID
```

## Phase 4: Test Execution Strategy

1. Fix test_helpers.h/cpp first
2. Fix each test file in order:
   - Audio (simplest)
   - Relay
   - RFID
   - Access Service
   - Integration (most complex)
3. Compile and run each test individually
4. Run all tests together

## Expected Outcomes

- All tests will use mock implementations consistently
- No direct hardware access in tests
- Clear separation between unit and integration tests
- Improved test maintainability and reliability
- Tests will properly verify business logic without hardware dependencies
