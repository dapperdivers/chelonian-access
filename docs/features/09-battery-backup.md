# Feature: Battery Backup

**Complexity**: 🟡 Medium
**Hardware Required**: 🏗️ Major (Battery, charging circuit)
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement battery backup to maintain access control functionality during power outages. This ensures security is maintained and authorized users can still enter during emergencies.

## Benefits

- Continuous operation during power outages
- Security maintained 24/7
- Emergency access preserved
- System status preservation
- Compliance with security standards

## Power Requirements Analysis

### Current Consumption Estimates
- ESP32-C3 SuperMini: ~100mA (WiFi active), ~20mA (idle), 43μA (deep sleep)
- PN532 RFID Reader: ~50mA active, ~10mA standby
- JQ6500 MP3 Player: ~20mA idle, ~200mA playing with speaker
- Relays (4x): ~80mA when active
- **Total**: ~50mA idle, ~350mA peak

### Battery Runtime Calculations
With deep sleep enabled (43μA base):
- 2000mAh battery: ~1000 hours deep sleep, ~40 hours idle, ~6 hours active
- 5000mAh battery: ~2500 hours deep sleep, ~100 hours idle, ~14 hours active
- 10000mAh battery: ~5000 hours deep sleep, ~200 hours idle, ~28 hours active

## Implementation Checklist

### Phase 1: Power Management System
- [ ] Create `PowerController` class:
  ```cpp
  class PowerController {
  private:
      gpio_num_t batteryPin;
      gpio_num_t chargerPin;
      gpio_num_t powerGoodPin;
      float batteryVoltage;
      bool onBattery;
      esp_adc_cal_characteristics_t adc_chars;
  public:
      void begin();
      float getBatteryVoltage();
      uint8_t getBatteryPercent();
      bool isOnBattery();
      bool isCharging();
      void enablePowerSaving();
      void calibrateADC();
  };
  ```
- [ ] ESP32-C3 ADC calibration
- [ ] Power source detection
- [ ] Automatic switchover

### Phase 2: Battery System Hardware
- [ ] **Battery Options**:
  - [ ] 18650 Li-ion cells (3.7V, 2000-3500mAh)
  - [ ] LiPo battery pack (3.7V-7.4V)
  - [ ] LiFePO4 (3.2V, safer chemistry)
  - [ ] USB power bank (5V output ready)

- [ ] **Charging Circuit**:
  - [ ] TP4056 for single Li-ion
  - [ ] CN3791 for solar + battery
  - [ ] IP5306 power bank IC
  - [ ] Protection circuit (overcharge/discharge)

### Phase 3: Power Switching
- [ ] **Automatic Switchover**:
  ```cpp
  // Power path using Mini360 buck converter
  12V Vehicle -----> Mini360 (5V) ----+
                                      |
                                    Diode
                                      |
  Battery Pack ----> Boost (5V) ---Diode----> System

  // Or use power management IC like LTC4412
  ```
- [ ] Zero-downtime switching
- [ ] Reverse polarity protection
- [ ] Automotive transient protection

### Phase 4: ESP32-C3 Power Optimization
- [ ] **Deep Sleep Implementation**:
  ```cpp
  void enterDeepSleep(uint64_t sleep_time_us) {
      // Configure wake sources
      esp_sleep_enable_timer_wakeup(sleep_time_us);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0); // Wake on button

      // Power down peripherals
      esp_wifi_stop();
      esp_bt_controller_disable();

      // Enter deep sleep (43μA)
      esp_deep_sleep_start();
  }
  ```
- [ ] **Dynamic Power Management**:
  ```cpp
  void adaptivePowerMode() {
      if (getBatteryPercent() < 20) {
          // Disable WiFi/BLE
          WiFi.mode(WIFI_OFF);
          btStop();

          // Reduce CPU frequency
          setCpuFrequencyMhz(80); // From 160MHz

          // Enable light sleep
          esp_pm_config_esp32c3_t pm_config = {
              .max_freq_mhz = 80,
              .min_freq_mhz = 10,
              .light_sleep_enable = true
          };
          esp_pm_configure(&pm_config);
      }
  }
  ```

### Phase 5: Battery Monitoring
- [ ] **ESP32-C3 ADC Usage**:
  ```cpp
  float readBatteryVoltage() {
      // Use ESP32 ADC with calibration
      uint32_t adc_reading = 0;

      // Multisampling for accuracy
      for (int i = 0; i < 64; i++) {
          adc_reading += adc1_get_raw(ADC1_CHANNEL_0);
      }
      adc_reading /= 64;

      // Convert to voltage with calibration
      uint32_t voltage = esp_adc_cal_raw_to_voltage(
          adc_reading, &adc_chars);

      // Account for voltage divider
      return (voltage / 1000.0) * DIVIDER_RATIO;
  }
  ```
- [ ] Low battery actions:
  - [ ] 30%: Disable WiFi/BLE
  - [ ] 20%: Reduce scan frequency
  - [ ] 10%: Audio alerts only
  - [ ] 5%: Deep sleep between scans
  - [ ] 2%: Save state and shutdown

### Phase 6: Solar Charging Option
- [ ] **Solar Integration**:
  ```cpp
  // MPPT solar charge controller
  Solar Panel -> CN3791 MPPT -> Battery
                     |
                     +-> System Power
  ```
- [ ] 5W-10W solar panel
- [ ] MPPT efficiency tracking
- [ ] Weather-resistant mounting

### Phase 7: Emergency Power Mode
- [ ] **Critical Battery Response**:
  ```cpp
  void emergencyPowerMode() {
      // ESP32-C3 specific optimizations

      // Disable all non-essential peripherals
      esp_wifi_stop();
      btStop();

      // Keep only RFID functional
      audio.disable();
      digitalWrite(8, LOW); // Blue LED off

      // Use ULP for ultra-low power monitoring
      if (batteryPercent < 2) {
          // Save state to NVS
          preferences.putBytes("last_state", &system_state, sizeof(system_state));

          // Enter hibernation (5μA)
          esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
          esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
          esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
          esp_deep_sleep_start();
      }
  }
  ```

## Hardware Design

### ESP32-C3 Power Architecture
```
Vehicle 12V -> Mini360 (5V) -> System 5V Bus
                                    |
                                    +-> ESP32-C3 (3.3V internal LDO)
                                    +-> Relays (5V)
                                    +-> PN532 (3.3V/5V tolerant)
                                    +-> JQ6500 (3.3V-5V)

Battery -> Protection -> Boost/Buck -> 5V Bus
```

### Battery Management System
```
Battery + ----[Fuse]---- BMS IC ---- Charge +
              |            |
              +--[Temp]----+
              |
Battery - ----+--------- BMS IC ---- Charge -
```

## Testing Checklist

- [ ] Power failure simulation
- [ ] Deep sleep current measurement (target: 43μA)
- [ ] Wake-up response time
- [ ] Battery runtime at various modes
- [ ] Solar charging efficiency
- [ ] Temperature compensation
- [ ] Automotive transient testing
- [ ] Long-term reliability

## Safety Considerations

- [ ] Automotive-grade components
- [ ] Temperature monitoring (ESP32 internal sensor)
- [ ] Overcurrent protection
- [ ] Reverse polarity protection
- [ ] ESD protection on all inputs
- [ ] Conformal coating for moisture

## Power Consumption Optimization

### Mode Comparison
| Mode | ESP32-C3 | PN532 | JQ6500 | Total |
|------|----------|--------|---------|--------|
| Deep Sleep | 43μA | Off | Off | 43μA |
| Light Sleep | 0.8mA | 10mA | Off | 11mA |
| Idle | 20mA | 10mA | 20mA | 50mA |
| Active | 100mA | 50mA | 200mA | 350mA |

## Cost Analysis

### Basic System (24-hour backup)
- 18650 battery holder: $3
- 18650 batteries (2x): $10
- TP4056 charger: $2
- Protection circuit: $3
- Boost converter: $5
- **Total: ~$23**

### Advanced System (7-day backup with solar)
- LiFePO4 pack (10Ah): $45
- CN3791 MPPT charger: $8
- 10W solar panel: $25
- Power management: $10
- Enclosure/mounting: $12
- **Total: ~$100**

### Power Bank Solution
- 20000mAh USB power bank: $30
- USB-C PD trigger board: $5
- Backup switching circuit: $5
- **Total: ~$40**

## Future Enhancements

- Supercapacitor for instant backup
- Energy harvesting from vehicle alternator
- Wireless charging capability
- Cloud-based power monitoring
- Predictive battery health analytics
- Integration with vehicle CAN bus
