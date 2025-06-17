---
title: Project Overview
nav_order: 2
---

# Chelonian Access System - Complete Project Overview

## Project Summary

Chelonian Access is a modern, security-focused RFID access control system built on the ESP32-C3 SuperMini platform. The system provides secure keyless entry with audio feedback, expandable relay control, and wireless connectivity capabilities.

## Documentation Index

### Core Documentation
- **[README.md](../README.md)** - Quick start guide and basic setup
- **[HARDWARE_GUIDE.md](./HARDWARE_GUIDE.md)** - Detailed component specifications and capabilities
- **[CURRENT_FEATURES.md](./CURRENT_FEATURES.md)** - What's implemented vs. what's possible

### Feature Enhancement Documentation
- **[features/README.md](./features/README.md)** - Overview of 12 proposed enhancements
- **[features/feature-roadmap.md](./features/feature-roadmap.md)** - Implementation timeline and priorities

## System Architecture

```
┌─────────────────┐     ┌──────────────────┐
│   RFID Card     │────▶│  PN532 Reader    │
└─────────────────┘     └────────┬─────────┘
                                 │ SPI
                        ┌────────▼─────────┐
                        │ ESP32-C3 Super   │
                        │     Mini          │
                        │ WiFi + Bluetooth  │
                        └──┬──────────┬────┘
                           │          │
                ┌──────────▼───┐   ┌──▼──────────┐
                │  4x Relays   │   │   JQ6500    │
                │ SRD-05VDC    │   │ MP3 Player  │
                └──────────────┘   └─────────────┘
                       │                  │
                ┌──────▼──────┐    ┌─────▼─────┐
                │ Door Locks  │    │  Speaker  │
                └─────────────┘    └───────────┘

                    ┌─────────────┐
                    │  Mini360    │
                    │ Buck Conv.  │
                    │ 12V → 5V    │
                    └─────────────┘
```

## Current System Capabilities

### What's Working Now
1. **Access Control**
   - Reads RFID cards (4-byte and 7-byte UIDs)
   - Validates against authorized list
   - Activates door unlock relay for 10 seconds
   - Brute-force protection with progressive delays

2. **Audio Feedback**
   - Power-up confirmation
   - Waiting prompt after 10 seconds
   - Access granted/denied sounds
   - Multiple denial levels for repeat attempts

3. **Software Architecture**
   - Modular OOP design
   - Comprehensive unit testing
   - Mock objects for development
   - Clean separation of concerns

### What's Possible with Current Hardware

The system currently uses only about 20% of the hardware's capabilities. Major untapped features include:

1. **Wireless Connectivity**
   - WiFi 802.11 b/g/n for remote management
   - Bluetooth 5.0 (BLE) for mobile app integration
   - Over-the-air (OTA) firmware updates
   - Web-based configuration portal
   - MQTT integration for home automation

2. **Power Efficiency**
   - Deep sleep mode (43μA consumption)
   - Wake-on-approach capabilities
   - Months of battery operation possible
   - Real-time power monitoring

3. **Enhanced Security**
   - Write data to RFID cards
   - Master card programming
   - Time-based access control
   - Multi-factor authentication
   - Remote access management

4. **User Experience**
   - Built-in blue LED for status
   - 4MB flash for extensive logging
   - JQ6500 audio flexibility
   - Multiple relay controls
   - Web dashboard

## Implementation Roadmap

### Phase 1: Essential Software Features (1-2 weeks)
1. **LED Status Indicators** - Visual feedback using onboard blue LED
2. **Low Power Sleep Mode** - Ultra-low 43μA deep sleep
3. **Master Card Programming** - Add/remove cards without computer
4. **Emergency Override** - Safety button for emergencies

### Phase 2: Wireless Features (1-2 weeks)
5. **WiFi Configuration Portal** - Easy setup without programming
6. **BLE Mobile App** - Use smartphone as key
7. **OTA Updates** - Firmware updates without physical access
8. **MQTT Integration** - Connect to home automation

### Phase 3: Enhanced Security (2-3 weeks)
9. **Multiple Access Levels** - Different permissions per card
10. **Access Logging** - Track all entry attempts
11. **Time-Based Access** - Schedule restrictions
12. **Remote Management** - Web-based administration

### Phase 4: Advanced Features (3-4 weeks)
13. **Battery Backup** - Uninterrupted operation
14. **Dual Authentication** - Two-factor security
15. **Mesh Networking** - Multiple access points
16. **Cloud Integration** - Remote monitoring

## Quick Start Improvements

If you want to enhance the system today with no additional hardware:

1. **Enable LED feedback** (30 minutes)
   ```cpp
   pinMode(8, OUTPUT);  // Blue LED
   digitalWrite(8, HIGH); // LED on
   ```

2. **Add WiFi portal** (2 hours)
   ```cpp
   #include <WiFi.h>
   #include <WebServer.h>
   // ESP32-C3 has built-in WiFi!
   ```

3. **Implement deep sleep** (1 hour)
   - ESP32-C3 achieves 43μA in deep sleep
   - Wake on GPIO or timer

## Hardware Investment Guide

### Zero Cost Improvements
- Blue LED indicator (built-in on GPIO8)
- 4MB flash storage
- WiFi access point
- Bluetooth connectivity
- Deep sleep modes

### Low Cost Additions ($5-20)
- Emergency button: $2
- Capacitive touch sensor: $1
- RTC module: $5
- External antenna: $5
- PIR motion sensor: $3

### Medium Cost Additions ($20-50)
- Battery backup system: $20-30
- OLED display: $10
- Solar charging: $25
- Keypad: $10
- GPS module: $15

## Development Setup

### Required Tools
- PlatformIO (VS Code extension recommended)
- USB-C cable
- ESP32-C3 board support

### Build Commands
```bash
# Build the project
pio run

# Upload to ESP32-C3
pio run -t upload

# Run unit tests
pio test

# Monitor serial output
pio device monitor
```

## Key Hardware Advantages

### ESP32-C3 SuperMini Benefits:
- **20x faster** than Pro Micro (160MHz vs 8MHz)
- **160x more RAM** (400KB vs 2.5KB)
- **125x more storage** (4MB vs 32KB)
- **Built-in wireless** (WiFi + Bluetooth)
- **Ultra-low power** (43μA deep sleep)
- **Tiny size** (22.52 x 18mm)

### JQ6500 Audio Benefits:
- Simple serial protocol
- Reliable operation
- Direct speaker drive
- Good documentation

### Mini360 Power Benefits:
- 96% efficiency
- Handles 12V automotive power
- Adjustable output
- Compact size
- Over-current protection

## Safety Warnings

⚠️ **IMPORTANT**: The ESP32-C3 operates at 3.3V logic levels. The system uses 5V power through the Mini360 converter for relay compatibility.

⚠️ **CRITICAL**: Ensure proper voltage settings on the Mini360 buck converter before connecting to the system.

## Support and Contributions

This project is open source and welcomes contributions. Areas where help is especially appreciated:

1. Testing wireless features
2. Mobile app development (iOS/Android)
3. Web dashboard UI
4. PCB design for permanent installation
5. 3D printed enclosure designs
6. Security auditing

## Future Vision

With the ESP32-C3 platform, this project can evolve into a comprehensive vehicle access and monitoring system:

- Remote start integration
- GPS tracking
- Fleet management
- Voice control via Alexa/Google
- Smartphone notifications
- Cloud-based analytics
- Multi-vehicle synchronization
- Integration with smart home systems

The powerful ESP32-C3 provides the foundation for endless possibilities while maintaining low power consumption and a tiny footprint!
