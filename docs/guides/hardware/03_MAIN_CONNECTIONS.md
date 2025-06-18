---
title: Main Connections
parent: Hardware Guides
nav_order: 4
---

# Main Connections

This guide covers the main power and signal connections between components in the Chelonian Access system.

## Required Components
- Mini360 Buck Converter
- Jumper wires
- Common ground wire
- Wire strippers
- Heat shrink tubing
- Multimeter

## Connection Steps

### 1. Power Distribution Setup

1. **Buck Converter Connections:**
```txt
Mini360 Buck Converter
┌─────────────────┐
│  IN+ ────────── │──── 12V Source (+)
│  IN- ────────── │──── Ground Bus
│  OUT+ ───────── │──── 5V Distribution
│  OUT- ───────── │──── Ground Bus
└─────────────────┘
```

2. **Ground Bus Setup:**
   - Establish common ground point
   - Connect all module grounds
   - Use appropriate wire gauge
   - Keep connections short

3. **5V Distribution:**
   - Connect 5V to required modules
   - Verify voltage at each point
   - Use appropriate wire gauge
   - Label all connections

### Best Practices
- Use star ground topology
- Keep power traces short
- Use proper wire gauge
- Label all connections
- Add strain relief

### Testing
1. Check voltage at all points
2. Verify ground connections
3. Test for voltage drops
4. Check for ground loops

### Troubleshooting
- Verify all ground connections
- Check for voltage drops
- Look for loose connections
- Test under maximum load
