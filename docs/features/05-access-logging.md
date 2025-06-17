# Feature: Access Logging

**Complexity**: 🟡 Medium
**Hardware Required**: ✅ None (uses ESP32-C3 flash)
**User Value**: ⭐⭐⭐ Essential

## Overview

Implement comprehensive logging of all access attempts, both successful and failed. Store logs locally using ESP32-C3's flash memory (NVS/SPIFFS), with options for remote transmission via built-in WiFi. Essential for security auditing and troubleshooting.

## Benefits

- Security audit trail
- Usage pattern analysis
- Troubleshooting system issues
- Compliance with security policies
- Evidence in case of security incidents

## Storage Options

### NVS (Non-Volatile Storage)
- Built into ESP32-C3
- 24KB available
- Perfect for configuration and recent events
- Survives deep sleep and power cycles
- ~400 log entries capacity

### SPIFFS (Flash File System)
- Built into ESP32-C3
- 1.5MB available
- File-based storage
- CSV export capability
- ~75,000 log entries capacity

### Remote Storage (WiFi)
- Real-time transmission
- Unlimited capacity
- Cloud integration options
- MQTT/HTTP/WebSocket support

### SD Card Module (Optional)
- Massive storage capacity
- Removable for analysis
- Requires SPI connection
- ~$5 for module + SD card

## Implementation Checklist

### Phase 1: Log Entry Structure
- [ ] Define log entry format:
  ```cpp
  struct LogEntry {
      uint32_t timestamp;      // Unix timestamp
      uint8_t eventType;       // Access granted/denied/error
      uint8_t uid[10];        // Card UID (up to 10 bytes)
      uint8_t uidLength;      // 4 or 7 bytes typically
      uint8_t accessLevel;    // If using access levels
      uint8_t relayActivated; // Which relay was triggered
      int8_t rssi;           // WiFi signal strength
      uint8_t batteryLevel;   // Battery percentage
      uint16_t checksum;      // Data integrity check
  };
  ```
- [ ] Event type constants:
  ```cpp
  enum EventType {
      EVENT_ACCESS_GRANTED = 0x01,
      EVENT_ACCESS_DENIED = 0x02,
      EVENT_INVALID_CARD = 0x03,
      EVENT_EXPIRED_CARD = 0x04,
      EVENT_TIME_RESTRICTED = 0x05,
      EVENT_EMERGENCY_UNLOCK = 0x06,
      EVENT_REMOTE_UNLOCK = 0x07,
      EVENT_BLE_UNLOCK = 0x08,
      EVENT_WEB_UNLOCK = 0x09,
      EVENT_SYSTEM_STARTUP = 0x0A,
      EVENT_SYSTEM_ERROR = 0x0B,
      EVENT_TAMPER_ALERT = 0x0C,
      EVENT_OTA_UPDATE = 0x0D,
      EVENT_CONFIG_CHANGE = 0x0E
  };
  ```

### Phase 2: ESP32-C3 Storage Implementation
- [ ] Create `LogController` class:
  ```cpp
  class LogController {
  private:
      Preferences preferences;
      const char* nvs_namespace = "access_log";
      File spiffs_log;
      uint32_t log_index;

  public:
      bool begin();
      bool logEvent(const LogEntry& entry);
      uint32_t getLogCount();
      bool readLog(uint32_t index, LogEntry& entry);
      bool exportToCSV(const char* filename);
      bool clearLogs();
      bool syncToCloud();
  };
  ```

### Phase 3: NVS Implementation
- [ ] Circular buffer in NVS:
  ```cpp
  bool LogController::logToNVS(const LogEntry& entry) {
      preferences.begin(nvs_namespace, false);

      // Get current index
      uint32_t index = preferences.getUInt("index", 0);

      // Store entry
      char key[16];
      snprintf(key, sizeof(key), "log_%d", index % MAX_NVS_LOGS);
      preferences.putBytes(key, &entry, sizeof(entry));

      // Update index
      preferences.putUInt("index", index + 1);
      preferences.putUInt("count", min(index + 1, MAX_NVS_LOGS));

      preferences.end();
      return true;
  }
  ```

### Phase 4: SPIFFS Implementation
- [ ] File-based logging:
  ```cpp
  bool LogController::logToSPIFFS(const LogEntry& entry) {
      // Create monthly log files
      char filename[32];
      struct tm timeinfo;
      getLocalTime(&timeinfo);
      strftime(filename, sizeof(filename), "/logs/%Y-%m.log", &timeinfo);

      // Append binary entry
      File file = SPIFFS.open(filename, FILE_APPEND);
      if (!file) return false;

      size_t written = file.write((uint8_t*)&entry, sizeof(entry));
      file.close();

      // Check if we need rotation
      checkLogRotation();

      return written == sizeof(entry);
  }
  ```

### Phase 5: Remote Logging
- [ ] WiFi transmission:
  ```cpp
  bool LogController::sendToServer(const LogEntry& entry) {
      if (!WiFi.isConnected()) return false;

      // Format as JSON
      StaticJsonDocument<256> doc;
      doc["timestamp"] = entry.timestamp;
      doc["event"] = getEventName(entry.eventType);
      doc["uid"] = uidToString(entry.uid, entry.uidLength);
      doc["rssi"] = WiFi.RSSI();

      // Send via HTTP POST
      HTTPClient http;
      http.begin(LOG_SERVER_URL);
      http.addHeader("Content-Type", "application/json");

      String json;
      serializeJson(doc, json);
      int httpCode = http.POST(json);
      http.end();

      return httpCode == 200;
  }
  ```

### Phase 6: Log Management
- [ ] **Web Interface**:
  ```cpp
  void setupLogWebEndpoints() {
      // View logs
      server.on("/api/logs", HTTP_GET, []() {
          String json = "[";
          LogEntry entry;

          for (int i = 0; i < getLogCount(); i++) {
              if (readLog(i, entry)) {
                  if (i > 0) json += ",";
                  json += entryToJSON(entry);
              }
          }
          json += "]";

          server.send(200, "application/json", json);
      });

      // Export CSV
      server.on("/api/logs/export", HTTP_GET, []() {
          server.sendHeader("Content-Disposition", "attachment; filename=logs.csv");
          server.send(200, "text/csv", exportLogsAsCSV());
      });
  }
  ```

### Phase 7: Analytics & Reporting
- [ ] **Built-in Analytics**:
  ```cpp
  struct LogStats {
      uint32_t totalAccess;
      uint32_t uniqueUsers;
      uint32_t failedAttempts;
      uint32_t peakHour;
      float avgDoorOpenTime;

      void calculate() {
          // Process logs in SPIFFS
          File root = SPIFFS.open("/logs");
          File file = root.openNextFile();

          while (file) {
              processLogFile(file);
              file = root.openNextFile();
          }
      }
  };
  ```

### Phase 8: Log Format Examples

#### CSV Export Format
```csv
Timestamp,Date,Time,Event,UID,User,AccessLevel,Relay,RSSI,Battery,Details
1704067200,2024-01-01,12:00:00,ACCESS_GRANTED,04:12:34:56:78:9A:BC,John Doe,USER,1,-45,85%,Door opened
1704067230,2024-01-01,12:00:30,ACCESS_DENIED,01:23:45:67,Unknown,NONE,0,-52,84%,Invalid card
1704067260,2024-01-01,12:01:00,REMOTE_UNLOCK,WEB_USER,Admin,ADMIN,1,-40,84%,Web interface
```

#### JSON Format (for remote transmission)
```json
{
  "timestamp": 1704067200,
  "event": "ACCESS_GRANTED",
  "uid": "04:12:34:56:78:9A:BC",
  "user": "John Doe",
  "accessLevel": "USER",
  "relay": 1,
  "rssi": -45,
  "battery": 85,
  "location": "Front Door"
}
```

## Memory Calculations

### NVS (24KB available)
- Header: 100 bytes
- Per entry: 24 bytes
- Capacity: ~400 entries

### SPIFFS (1.5MB available)
- System files: ~100KB
- Per entry: 24 bytes (binary)
- Per entry: ~100 bytes (CSV)
- Capacity: ~50,000 entries (binary)

### Cloud Storage
- Unlimited capacity
- Real-time access
- Advanced analytics
- Multi-site aggregation

## Testing Checklist

- [ ] Flash write performance
- [ ] Deep sleep persistence
- [ ] Power loss during write
- [ ] SPIFFS fragmentation handling
- [ ] WiFi transmission reliability
- [ ] Time sync accuracy (NTP)
- [ ] Web export functionality
- [ ] Log rotation testing
- [ ] Memory usage monitoring

## Privacy & Security

- [ ] Encrypted storage option
- [ ] GDPR compliance features
- [ ] Automatic anonymization
- [ ] Role-based log access
- [ ] Audit log of log access
- [ ] Secure transmission (HTTPS)
- [ ] Data retention policies

## Power Optimization

- [ ] Batch log uploads
- [ ] Compress before transmission
- [ ] Use MQTT for efficiency
- [ ] Cache during WiFi outage
- [ ] Deep sleep between uploads

## Future Enhancements

- WebSocket real-time streaming
- Machine learning anomaly detection
- Integration with security systems
- Blockchain for tamper-proof logs
- Edge analytics processing
- Multi-site synchronization
- Predictive access patterns
