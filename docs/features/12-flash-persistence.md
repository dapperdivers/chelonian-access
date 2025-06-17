# Feature: Flash Storage Persistence

**Complexity**: 🟢 Low
**Hardware Required**: ✅ None (uses ESP32-C3's 4MB flash)
**User Value**: ⭐⭐⭐ Essential

## Overview

Leverage the ESP32-C3 SuperMini's 4MB flash memory for data persistence, storing authorized UIDs, access logs, and configuration settings. This provides ample storage without additional hardware, using the ESP32's built-in flash memory management capabilities.

## Benefits

- No additional hardware needed
- 4MB of flash storage available
- Survives power cycles and reboots
- Built-in wear leveling
- Fast read/write operations
- Partition-based organization
- OTA-safe storage areas

## ESP32-C3 Flash Architecture

### Flash Layout
```
Total Flash: 4MB
├── Bootloader: 0x1000 (4KB)
├── Partition Table: 0x8000 (4KB)
├── NVS: 0x9000 (24KB) - Non-volatile storage
├── OTA Data: 0xF000 (8KB)
├── App0: 0x10000 (1.2MB) - Main application
├── App1: 0x150000 (1.2MB) - OTA update partition
├── SPIFFS: 0x290000 (1.5MB) - File system
└── Reserved: Remaining space
```

## Implementation Checklist

### Phase 1: NVS (Preferences) Storage
- [ ] Create `FlashStorage` class using Preferences library:
  ```cpp
  #include <Preferences.h>

  class FlashStorage {
  private:
      Preferences preferences;
      const char* namespace_config = "config";
      const char* namespace_uids = "uids";
      const char* namespace_logs = "logs";

  public:
      void begin();

      // Configuration
      bool saveConfig(const SystemConfig& config);
      bool loadConfig(SystemConfig& config);

      // UID Management
      bool saveUID(const char* key, const uint8_t* uid, size_t length);
      bool removeUID(const char* key);
      size_t getUIDCount();
      bool getUID(size_t index, uint8_t* uid, size_t& length);

      // Access Logging
      bool logAccess(const AccessLog& entry);
      size_t getLogCount();
      bool getLog(size_t index, AccessLog& entry);
  };
  ```

### Phase 2: SPIFFS File System
- [ ] Initialize SPIFFS for larger data:
  ```cpp
  #include <SPIFFS.h>

  class SPIFFSStorage {
  private:
      const size_t MAX_FILE_SIZE = 1048576; // 1MB

  public:
      bool begin() {
          if (!SPIFFS.begin(true)) {
              Serial.println("SPIFFS Mount Failed");
              return false;
          }

          // Check available space
          size_t totalBytes = SPIFFS.totalBytes();
          size_t usedBytes = SPIFFS.usedBytes();
          Serial.printf("SPIFFS: %d/%d bytes used\n", usedBytes, totalBytes);
          return true;
      }

      bool writeFile(const char* path, const uint8_t* data, size_t len);
      bool readFile(const char* path, uint8_t* data, size_t& len);
      bool deleteFile(const char* path);
      bool listDir(const char* dirname);
  };
  ```

### Phase 3: Data Structure Design
- [ ] Define storage structures:
  ```cpp
  // System configuration
  struct SystemConfig {
      uint32_t magic = 0xC3RFID00;  // Magic number for validation
      uint8_t version = 1;          // Config version
      uint8_t volume = 10;          // Audio volume
      uint16_t relayTime = 10000;   // Door unlock duration (ms)
      uint32_t sleepTimeout = 30000; // Auto-sleep delay (ms)
      uint8_t wifiEnabled = 1;      // WiFi on/off
      char wifiSSID[32];            // WiFi network name
      char wifiPass[64];            // WiFi password
      uint8_t bleEnabled = 1;       // Bluetooth on/off
      char blePIN[7] = "123456";    // BLE pairing PIN
  };

  // UID storage entry
  struct UIDEntry {
      uint8_t uid[10];              // UID bytes (supports up to 10)
      uint8_t length;               // Actual UID length
      uint8_t accessLevel;          // Access permissions
      char name[32];                // Card holder name
      uint32_t validFrom;           // Validity start (epoch)
      uint32_t validUntil;          // Validity end (epoch)
  };

  // Access log entry
  struct AccessLog {
      uint32_t timestamp;           // Event time (epoch)
      uint8_t uid[10];             // Card UID
      uint8_t uidLength;           // UID length
      uint8_t granted;             // Access granted/denied
      uint8_t reason;              // Denial reason code
  };
  ```

### Phase 4: Preferences Implementation
- [ ] Basic storage operations:
  ```cpp
  void FlashStorage::begin() {
      // Initialize with false for read/write mode
      preferences.begin(namespace_config, false);
  }

  bool FlashStorage::saveUID(const char* key, const uint8_t* uid, size_t length) {
      preferences.begin(namespace_uids, false);

      // Store UID with metadata
      char uidKey[32];
      snprintf(uidKey, sizeof(uidKey), "uid_%s", key);
      size_t written = preferences.putBytes(uidKey, uid, length);

      // Store UID count
      size_t count = preferences.getUInt("count", 0);
      preferences.putUInt("count", count + 1);

      preferences.end();
      return written == length;
  }

  bool FlashStorage::saveConfig(const SystemConfig& config) {
      preferences.begin(namespace_config, false);
      size_t written = preferences.putBytes("config", &config, sizeof(config));
      preferences.end();
      return written == sizeof(config);
  }
  ```

### Phase 5: SPIFFS Implementation
- [ ] File-based storage for logs:
  ```cpp
  bool SPIFFSStorage::logAccess(const AccessLog& entry) {
      // Create monthly log files
      char filename[32];
      struct tm timeinfo;
      getLocalTime(&timeinfo);
      strftime(filename, sizeof(filename), "/logs/%Y-%m.log", &timeinfo);

      // Append to log file
      File file = SPIFFS.open(filename, FILE_APPEND);
      if (!file) {
          return false;
      }

      size_t written = file.write((uint8_t*)&entry, sizeof(entry));
      file.close();

      return written == sizeof(entry);
  }

  bool SPIFFSStorage::exportLogsCSV(const char* month) {
      char logFile[32];
      snprintf(logFile, sizeof(logFile), "/logs/%s.log", month);

      char csvFile[32];
      snprintf(csvFile, sizeof(csvFile), "/export/%s.csv", month);

      File input = SPIFFS.open(logFile, FILE_READ);
      File output = SPIFFS.open(csvFile, FILE_WRITE);

      // Write CSV header
      output.println("Timestamp,UID,Granted,Reason");

      // Convert binary logs to CSV
      AccessLog entry;
      while (input.read((uint8_t*)&entry, sizeof(entry)) == sizeof(entry)) {
          output.printf("%lu,%s,%d,%d\n",
              entry.timestamp,
              uidToString(entry.uid, entry.uidLength),
              entry.granted,
              entry.reason);
      }

      input.close();
      output.close();
      return true;
  }
  ```

### Phase 6: Web-Based Management
- [ ] Serve configuration UI from SPIFFS:
  ```cpp
  void handleFileUpload() {
      HTTPUpload& upload = server.upload();
      static File uploadFile;

      if (upload.status == UPLOAD_FILE_START) {
          String filename = upload.filename;
          if (!filename.startsWith("/")) filename = "/" + filename;
          uploadFile = SPIFFS.open(filename, FILE_WRITE);
      } else if (upload.status == UPLOAD_FILE_WRITE) {
          if (uploadFile) {
              uploadFile.write(upload.buf, upload.currentSize);
          }
      } else if (upload.status == UPLOAD_FILE_END) {
          if (uploadFile) {
              uploadFile.close();
          }
      }
  }
  ```

### Phase 7: Backup and Restore
- [ ] Export/Import functionality:
  ```cpp
  class BackupManager {
  public:
      bool exportToJSON() {
          File backup = SPIFFS.open("/backup.json", FILE_WRITE);
          if (!backup) return false;

          // Start JSON
          backup.print("{\n");

          // Export config
          backup.print("  \"config\": ");
          exportConfig(backup);
          backup.print(",\n");

          // Export UIDs
          backup.print("  \"uids\": ");
          exportUIDs(backup);
          backup.print("\n");

          backup.print("}\n");
          backup.close();
          return true;
      }

      bool importFromJSON() {
          File backup = SPIFFS.open("/backup.json", FILE_READ);
          if (!backup) return false;

          // Parse JSON and restore
          // ... JSON parsing code

          backup.close();
          return true;
      }
  };
  ```

## Testing Checklist

- [ ] Flash write endurance test
- [ ] Power loss during write
- [ ] Partition overflow handling
- [ ] SPIFFS fragmentation
- [ ] Concurrent access safety
- [ ] OTA update preservation
- [ ] Backup/restore integrity
- [ ] Performance benchmarks

## Configuration Example

```cpp
// Flash storage configuration
#define USE_NVS_CONFIG true      // Use NVS for config
#define USE_SPIFFS_LOGS true     // Use SPIFFS for logs
#define LOG_RETENTION_DAYS 365   // Keep 1 year of logs
#define MAX_UIDS_NVS 100        // UIDs in NVS
#define MAX_UIDS_SPIFFS 10000   // UIDs in SPIFFS if needed
#define ENABLE_WEB_CONFIG true   // Web-based management
```

## Storage Capacity

### NVS (24KB available)
- Configuration: ~200 bytes
- UIDs: ~50 bytes each
- **Capacity**: ~400 UIDs

### SPIFFS (1.5MB available)
- Log entries: ~20 bytes each
- **Capacity**: ~75,000 log entries
- Web files: ~100KB
- Backup files: ~50KB

## Advantages Over EEPROM

| Feature | EEPROM | ESP32 Flash |
|---------|---------|-------------|
| Capacity | 1-4KB | 4MB total |
| Speed | Slow | Fast |
| Wear Leveling | No | Yes |
| File System | No | Yes (SPIFFS) |
| Encryption | No | Yes (NVS) |
| OTA Safe | No | Yes |

## Future Enhancements

- Encrypted storage for sensitive data
- Cloud backup via WiFi
- Automatic log rotation
- Real-time sync with mobile app
- SD card support for unlimited storage
- Database export formats
