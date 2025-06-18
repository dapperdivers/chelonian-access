---
title: Audio System Installation
parent: Hardware Guides
nav_order: 7
---

# Audio System Installation

This guide covers the installation and setup of the JQ6500 MP3 Player Module and speaker system.

## Component Images

![JQ6500 Audio Module](../assets/img/assembly/JQ6500-audio.jpg)
*JQ6500 Audio Module*

## Required Components
- JQ6500 MP3 Player Module
- 8Ω Speaker
- Jumper wires
- Heat shrink tubing
- Mounting hardware

## Installation Steps

### 1. JQ6500 Module Setup

1. **Power Connections:**
   - Connect VCC to 5V from Mini360
   - Connect GND to ground bus
   - Verify power LED illumination
   - Check for stable voltage

2. **Communication Setup:**
   - Connect TX/RX to ESP32 pins 0/1
   - Keep wires short and away from noise
   - Use proper wire routing
   - Add labels to connections

### 2. Speaker Installation

1. **Mounting:**
   - Select appropriate location
   - Use rubber grommets to reduce vibration
   - Ensure water protection if needed
   - Create proper sound projection

2. **Wiring:**
   - Connect speaker to JQ6500 output
   - Use appropriate wire gauge
   - Add strain relief
   - Protect connections

### 3. Audio Testing

1. **Initial Setup:**
   - Set initial volume level
   - Test basic functionality
   - Check for clear sound
   - Verify no interference

2. **Fine Tuning:**
   - Adjust volume as needed
   - Check sound clarity
   - Test different positions
   - Verify audio feedback

### Best Practices
- Keep audio wires away from power
- Use shielded cable if possible
- Mount for clear sound projection
- Consider acoustic properties

### Testing
1. Test all sound files
2. Verify volume levels
3. Check for interference
4. Test in final position

### Troubleshooting
- Check power connections
- Verify TX/RX connections
- Test speaker directly
- Look for interference sources
