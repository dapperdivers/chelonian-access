---
title: Feature Guides
nav_order: 5
has_children: true
---

# New Feature Ideas for Chelonian Access

This directory contains detailed feature proposals for enhancing the RFID access control system. Each feature has its own document with implementation checklists.

## Feature List

1. **[LED Status Indicators](./01-led-status-indicators.md)** - Visual feedback using ESP32-C3 onboard LED
2. **[Multiple Access Levels](./02-multiple-access-levels.md)** - Different permission levels for different cards
3. **[Time-Based Access Control](./03-time-based-access.md)** - Schedule-based access restrictions
4. **[Remote Management](./04-remote-management.md)** - Built-in WiFi/Bluetooth configuration interface
5. **[Access Logging](./05-access-logging.md)** - Flash storage logging of access attempts
6. **[Master Card Programming](./06-master-card-programming.md)** - Add/remove cards without reprogramming
7. **[Dual Authentication](./07-dual-authentication.md)** - Require two cards for high-security access
8. **[Emergency Override](./08-emergency-override.md)** - Physical button or special card for emergencies
9. **[Battery Backup](./09-battery-backup.md)** - Maintain operation during power outages
10. **[Mobile App Integration](./10-mobile-app-integration.md)** - Smartphone control via built-in BLE
11. **[Low Power Sleep Mode](./11-low-power-sleep-mode.md)** - Ultra-low power consumption (43μA) with wake-on-touch
12. **[Flash Storage Persistence](./12-flash-persistence.md)** - Use ESP32-C3's 4MB flash for data storage
13. **[Configurable Relay Modes](./13-configurable-relay-modes.md)** - NO/NC, fail-safe/secure, timing patterns
14. **[Scheduled Relay Control](./14-scheduled-relay-control.md)** - Time-based relay on/off scheduling

## Implementation Priority

Features are ranked by:
- **Complexity**: Low 🟢, Medium 🟡, High 🔴
- **Hardware Required**: None ✅, Minor 🔧, Major 🏗️
- **User Value**: Essential ⭐⭐⭐, Nice-to-have ⭐⭐, Optional ⭐

## Getting Started

1. Review each feature document
2. Select features based on your needs and available hardware
3. Follow the implementation checklists
4. Test thoroughly before deployment