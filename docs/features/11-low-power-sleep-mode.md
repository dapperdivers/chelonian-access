# Feature: Low Power Sleep Mode

**Complexity**: 🟢 Low
**Hardware Required**: 🔧 Minor (Capacitive button or switch)
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement ultra-low power deep sleep mode for the ESP32-C3 SuperMini, reducing power consumption to just 43μA when idle. The system wakes instantly when a capacitive button is touched, when motion is detected, or on timer-based schedules, making it perfect for battery-powered installations.

## Benefits

- Dramatically extends battery life (months to years)
- Reduces power consumption by 99.9%+ when idle
- Multiple wake sources (GPIO, timer, touch)
- Instant wake-up response
- Perfect for solar/battery powered systems
- Built-in RTC for scheduled wake

## Power Consumption Analysis

### Active Mode
- WiFi active: ~100mA
- WiFi off, CPU active: ~20mA
- Daily consumption: ~0.5-2.4Ah

### Deep Sleep Mode
- Deep sleep: ~43μA (0.043mA)
- Daily consumption: ~0.001Ah
- **Power savings: 99.96%+**

## Implementation Checklist

### Phase 1: Basic Sleep Implementation
- [ ] Create `PowerSaveController` class:
  ```cpp
  class PowerSaveController {
  private:
      gpio_num_t wakePin;
      millis_t lastActivity;
      millis_t sleepTimeout;
      RTC_DATA_ATTR int bootCount = 0;
  public:
      void begin(gpio_num_t pin = GPIO_NUM_0);
      void checkAndSleep();
      void forceSleep();
      void enableWakeupByPin(gpio_num_t pin, int level);
      void enableWakeupByTimer(uint64_t time_us);
      void enableWakeupByTouch(touch_pad_t pad);
      bool isWakeFromSleep();
      esp_sleep_wakeup_cause_t getWakeupReason();
      void resetActivityTimer();
  };
  ```
- [ ] Implement ESP32 deep sleep API
- [ ] Configure wake sources

### Phase 2: Wake Trigger Hardware
- [ ] **Capacitive Touch Button**:
  - [ ] Use ESP32 built-in touch sensors
  - [ ] No external hardware needed!
  - [ ] Weather-resistant metal surface
  - [ ] Adjustable sensitivity

- [ ] **PIR Motion Sensor**:
  - [ ] HC-SR501 PIR sensor (~$2)
  - [ ] Connect to any GPIO
  - [ ] 3-7m detection range
  - [ ] Wake on approach

- [ ] **Simple Button**:
  - [ ] Momentary switch to GND
  - [ ] Internal pullup resistor
  - [ ] Any GPIO pin

### Phase 3: Sleep Mode Implementation
- [ ] Core sleep code for ESP32-C3:
  ```cpp
  #include <esp_sleep.h>
  #include <esp_pm.h>

  void PowerSaveController::forceSleep() {
      // Configure wake up sources
      esp_sleep_enable_ext0_wakeup(wakePin, 0);

      // Optional: Timer wakeup (e.g., every hour)
      // esp_sleep_enable_timer_wakeup(3600 * 1000000ULL);

      // Optional: Touch wakeup
      // esp_sleep_enable_touchpad_wakeup();

      // Store data in RTC memory
      bootCount++;

      // Enter deep sleep
      Serial.println("Going to sleep...");
      Serial.flush();
      esp_deep_sleep_start();
  }

  bool PowerSaveController::isWakeFromSleep() {
      return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED;
  }

  esp_sleep_wakeup_cause_t PowerSaveController::getWakeupReason() {
      esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

      switch(wakeup_reason) {
          case ESP_SLEEP_WAKEUP_EXT0:
              Serial.println("Wakeup by external signal (RTC_IO)");
              break;
          case ESP_SLEEP_WAKEUP_TIMER:
              Serial.println("Wakeup by timer");
              break;
          case ESP_SLEEP_WAKEUP_TOUCHPAD:
              Serial.println("Wakeup by touchpad");
              break;
          default:
              Serial.println("Wakeup not by deep sleep");
              break;
      }
      return wakeup_reason;
  }
  ```

### Phase 4: Peripheral Management
- [ ] **Before Sleep**:
  ```cpp
  void prepareForSleep() {
      // Turn off WiFi
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      // Turn off Bluetooth
      btStop();

      // Power down RFID (via GPIO or MOSFET)
      digitalWrite(RFID_POWER_PIN, LOW);

      // Mute audio
      digitalWrite(JQ6500_POWER_PIN, LOW);

      // Turn off LEDs
      digitalWrite(8, LOW); // Blue LED

      // Flush serial buffers
      Serial.flush();
  }
  ```

- [ ] **After Wake**:
  ```cpp
  void wakeupInit() {
      // Check wake reason
      esp_sleep_wakeup_cause_t wakeup_reason = getWakeupReason();

      // Re-initialize based on wake reason
      if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
          // Quick wake for card scan
          rfid.begin();
          audio.begin();
      } else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
          // Periodic wake - maybe check for OTA updates
          WiFi.begin(ssid, password);
      }
  }
  ```

### Phase 5: Activity-Based Sleep
- [ ] **Auto-Sleep Logic**:
  - [ ] Monitor last card scan time
  - [ ] Monitor last button press
  - [ ] Monitor last WiFi/BLE activity
  - [ ] Sleep after configurable timeout (e.g., 30 seconds)

- [ ] **Smart Wake Conditions**:
  - [ ] GPIO interrupt (button/PIR)
  - [ ] Touch sensor activation
  - [ ] Timer-based (hourly check-in)
  - [ ] ULP coprocessor events

### Phase 6: Touch Sensor Setup
- [ ] **Built-in Touch Support**:
  ```cpp
  void setupTouch() {
      // Initialize touch pad
      touchAttachInterrupt(T0, touchCallback, TOUCH_THRESHOLD);

      // Configure for sleep wakeup
      esp_sleep_enable_touchpad_wakeup();
  }

  void touchCallback() {
      // Touch detected - reset activity timer
      powerSave.resetActivityTimer();
  }
  ```

### Phase 7: Power Optimization
- [ ] **Dynamic Power Management**:
  ```cpp
  // Configure power management
  esp_pm_config_esp32c3_t pm_config = {
      .max_freq_mhz = 160,
      .min_freq_mhz = 10,
      .light_sleep_enable = true
  };
  esp_pm_configure(&pm_config);
  ```

- [ ] **Light Sleep for Short Delays**:
  ```cpp
  // Use light sleep instead of delay()
  vTaskDelay(100 / portTICK_PERIOD_MS);
  ```

## Hardware Connections

### Touch Sensor Wake (Built-in)
```
Metal Surface ---- 1MΩ resistor ---- GPIO (T0-T9)
                                      |
                                 ESP32-C3 Touch Pin
```

### PIR Motion Sensor Wake
```
ESP32 GPIO -------- HC-SR501 OUT
GND --------------- HC-SR501 GND
3.3V -------------- HC-SR501 VCC
```

### RFID Power Control
```
ESP32 GPIO ---- Gate (MOSFET) ---- RFID VCC
                      |
                    Source -------- 5V
                      |
                    Drain --------- RFID Power
```

## Testing Checklist

- [ ] Measure deep sleep current (~43μA)
- [ ] Test all wake sources
- [ ] Verify peripheral restart
- [ ] Test activity timeout
- [ ] Battery life calculation
- [ ] Temperature stability
- [ ] WiFi reconnection time
- [ ] Touch sensitivity tuning

## Configuration Options

```cpp
// Power save configuration
#define ENABLE_SLEEP_MODE true
#define SLEEP_TIMEOUT_MS 30000        // 30 seconds
#define WAKE_GPIO GPIO_NUM_0          // Button wake pin
#define TOUCH_THRESHOLD 40            // Touch sensitivity
#define PERIODIC_WAKE_HOURS 6         // Wake every 6 hours
#define MIN_BATTERY_VOLTAGE 3.0       // Sleep if battery low
#define USE_LIGHT_SLEEP_DELAY true    // Use light sleep in delays
```

## Power Consumption Examples

### Without Sleep Mode (24/7 operation)
- 50mA average × 24h = 1.2Ah/day
- 2000mAh battery = 1.7 days runtime

### With Deep Sleep (10 uses/day, 1min each)
- Active: 50mA × 10min = 8.3mAh
- Sleep: 0.043mA × 1430min = 1.0mAh
- Total: 9.3mAh/day
- 2000mAh battery = **215 days runtime**

### With Solar Panel (1W panel)
- Solar input: ~200mAh/day (average)
- Consumption: 9.3mAh/day
- **Indefinite runtime with surplus charging**

## ESP32-C3 Sleep Modes

| Mode | Current | Wake Time | Use Case |
|------|---------|-----------|----------|
| Active | 20-100mA | - | Normal operation |
| Modem Sleep | 15mA | <1ms | WiFi periodic |
| Light Sleep | 0.8mA | <1ms | Short pauses |
| Deep Sleep | 43μA | 3ms | Long idle periods |

## Troubleshooting

- **Won't Wake**: Check GPIO pullup/pulldown
- **High Sleep Current**: Ensure WiFi/BT off
- **Touch Too Sensitive**: Adjust threshold
- **Slow Wake**: Minimize reinit code

## Future Enhancements

- ULP coprocessor for ultra-low power sensing
- Solar MPPT charging integration
- LoRa wake-on-radio
- Accelerometer wake on vibration
- Scheduled wake with RTC
- Power harvesting from door movement
