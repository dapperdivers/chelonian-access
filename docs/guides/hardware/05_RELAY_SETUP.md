---
title: Relay Module Setup
parent: Hardware Guides
nav_order: 6
---

# Relay Module Setup

This guide covers the installation and configuration of the 4-Channel Relay Module (SRD-05VDC-SL-C) in the Chelonian Access system.

## Component Images

![4-Channel Relay Module](../assets/img/assembly/4-relay-module.jpeg)
*4-Channel Relay Module*

![Wiring Connections](../assets/img/assembly/simple sexy wired connections.jpeg)
*Relay Module Wiring Connections*

## Required Components
- 4-Channel Relay Module (SRD-05VDC-SL-C)
- Jumper wires
- Heat shrink tubing
- Wire strippers
- Screwdriver set

## Installation Steps

### 1. Module Preparation

1. **Initial Inspection:**
   - Check relay module for damage
   - Verify relay specifications
   - Identify control pins
   - Note COM/NO/NC terminals

2. **Power Setup:**
   - Connect VCC to 5V supply
   - Connect GND to common ground
   - Verify power LED illuminates
   - Check relay response

### 2. Control Wiring

1. **Signal Connections:**
   - Connect control pins to ESP32
   - Use appropriate wire gauge
   - Add pull-up/down resistors if needed
   - Label all connections

2. **Load Connections:**
   - Identify load requirements
   - Choose appropriate terminals (NO/NC)
   - Use proper wire gauge
   - Add strain relief

### Best Practices
- Keep high voltage separated
- Use proper wire gauge
- Add strain relief
- Label all connections
- Test before final mount

### Testing
1. Verify control signals
2. Test each relay channel
3. Check for proper switching
4. Listen for relay click
5. Measure contact resistance

### Troubleshooting
- Check power supply voltage
- Verify control signals
- Test relay manually
- Check for loose connections
