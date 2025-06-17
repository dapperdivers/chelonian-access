---
title: Hardware Assembly Guide
parent: Hardware Guides
nav_order: 1
---

# Hardware Assembly Guide

This guide provides step-by-step instructions for assembling the Chelonian Access system hardware components. Follow these instructions carefully to ensure proper functionality and optimal performance.

## Prerequisites

### Required Components
- ESP32-C3 SuperMini
- PN532 NFC/RFID Module
- 4-Channel Relay Module (SRD-05VDC-SL-C)
- JQ6500 MP3 Player Module
- Mini360 Buck Converter
- Speaker (8Ω, compatible with JQ6500)
- Jumper wires
- 12V power source
- Project enclosure
- Common ground wire
- Strain relief for cables

### Required Tools
- Soldering iron and solder
- Wire strippers
- Small screwdriver set
- Multimeter for testing
- Heat shrink tubing
- Zip ties for cable management

## Assembly Steps

### 1. Power Supply Setup

1. **Configure Mini360 Buck Converter:**
   - Connect a multimeter to OUT+ and OUT-
   - Apply 12V to IN+ and IN- (from vehicle or power supply)
   - Adjust the trim pot until output reads exactly 5.0V
   - Double-check voltage stability under load

### 2. Core Board Preparation

1. **ESP32-C3 SuperMini:**
   - Inspect the board for any damage
   - If headers aren't pre-soldered, solder them now
   - Mark pin numbers for easy reference

2. **PN532 Module Setup:**
   - Set jumpers for SPI mode operation
   - Verify the module is set to 3.3V operation
   - Solder headers if needed

### 3. Main Connections

1. **Power Distribution:**
```txt
Mini360 Buck Converter
┌─────────────────┐
│  IN+ ────────── │──── 12V Source (+)
│  IN- ────────── │──── Ground Bus
│  OUT+ ───────── │──── 5V Distribution
│  OUT- ───────── │──── Ground Bus
└─────────────────┘
```

2. **ESP32-C3 SuperMini Connections:**
```txt
ESP32-C3 SuperMini
┌─────────────────┐
│                 │
│  10 ─────────── │──── PN532 SS
│  MISO ────────── │──── PN532 MISO
│  MOSI ────────── │──── PN532 MOSI
│  SCK ─────────── │──── PN532 SCK
│                 │
│  9 ──────────── │──── Relay 1 (Door)
│  6 ──────────── │──── Relay 2
│  5 ──────────── │──── Relay 3
│  4 ──────────── │──── Relay 4
│                 │
│  1 ──────────── │──── JQ6500 RX
│  0 ──────────── │──── JQ6500 TX
│                 │
│  5V ─────────── │──── Mini360 OUT+
│  GND ─────────── │──── Ground Bus
└─────────────────┘
```

### 4. RFID Reader Installation

1. **PN532 Module Connections:**
   - Keep SPI wires as short as possible
   - Use shielded cable if available
   - Mount antenna away from metal surfaces
   - Keep at least 10cm from other electronics
   - Test read range after mounting

### 5. Relay Module Setup

1. **Relay Board Connections:**
   - Connect VCC to 5V from Mini360
   - Connect GND to ground bus
   - Connect IN1-IN4 to ESP32 pins 9,6,5,4
   - Use flyback diodes if not built into module
   - Test each relay with multimeter (continuity)

### 6. Audio System Installation

1. **JQ6500 Module:**
   - Connect VCC to 5V from Mini360
   - Connect GND to ground bus
   - Connect TX/RX to ESP32 pins 0/1
   - Test speaker connection before final mount
   - Set volume to appropriate level

2. **Speaker Mounting:**
   - Mount in enclosure for sound projection
   - Use rubber grommets to reduce vibration
   - Ensure water protection if needed
   - Test sound clarity after mounting

### 7. Final Assembly

1. **Enclosure Preparation:**
   - Drill/cut holes for:
     - RFID antenna (minimize metal around it)
     - Status LEDs
     - Speaker grille
     - Cable entry points
     - Mounting points

2. **Component Mounting:**
   - Mount boards using standoffs
   - Ensure good airflow around components
   - Keep high-current wires away from signal wires
   - Use cable ties for strain relief
   - Label all connections

3. **Testing:**
   - Check all power connections
   - Verify ground connections
   - Test RFID read range
   - Test all relay operations
   - Verify audio feedback
   - Check for interference between components

## Best Practices

1. **Power Supply:**
   - Mini360 provides stable 5V from automotive 12V
   - Double-check voltage under load
   - Consider adding fuse protection

2. **Wiring:**
   - Keep SPI lines short for RFID reliability
   - Use appropriate wire gauge for power
   - Label all connections
   - Use ferrules on stranded wire ends

3. **Grounding:**
   - Use common ground point (star ground)
   - Ensure clean ground connections
   - Check for ground loops

4. **Protection:**
   - Use strain relief on all cables
   - Protect from water/dust as needed
   - Consider conformal coating for boards
   - Add ventilation if enclosed

5. **Testing:**
   - Test each component individually
   - Verify all connections before power-up
   - Document any modifications
   - Keep spare components handy

## Troubleshooting

Common issues and solutions:

1. **RFID Not Reading:**
   - Check SPI connections
   - Verify power supply voltage
   - Check for metal interference
   - Test different card positions

2. **Relay Issues:**
   - Verify 5V supply voltage
   - Check control signal connections
   - Test relay manually
   - Listen for clicking sound

3. **Audio Problems:**
   - Check TX/RX connections
   - Verify speaker connections
   - Test volume settings
   - Check for timing issues

4. **Power Issues:**
   - Verify Mini360 output voltage
   - Check for voltage drops
   - Look for loose connections
   - Test under load

## Post-Installation

1. **Final Checks:**
   - Verify all connections
   - Test all functionalities
   - Check for loose components
   - Ensure proper closure

2. **Documentation:**
   - Note any modifications
   - Record voltage measurements
   - Document wire colors/connections
   - Keep wiring diagram accessible

3. **Maintenance:**
   - Regular inspection schedule
   - Clean contacts as needed
   - Check for loose connections
   - Monitor voltage levels

## Safety Considerations

1. **Electrical Safety:**
   - Proper fusing for power input
   - Strain relief on all cables
   - Proper wire gauge selection
   - Protection from short circuits

2. **Physical Safety:**
   - Secure mounting
   - No sharp edges
   - Proper enclosure
   - Environmental protection

3. **Operation Safety:**
   - Emergency override access
   - Backup power considerations
   - Fail-safe relay configuration
   - Regular testing procedure

## Next Steps

After completing the hardware assembly:

1. **Software Setup:**
   - Follow the software installation guide
   - Configure default settings
   - Test all functions
   - Set up master cards

2. **Testing:**
   - Full system test
   - Range testing
   - Reliability testing
   - Environmental testing

3. **Documentation:**
   - Record any modifications
   - Note testing results
   - Keep maintenance log
   - Schedule regular checks
