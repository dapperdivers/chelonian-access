#ifndef UNIT_TEST

#include <Arduino.h>
#include "audio_player.h"
#include "relay_controller.h"
#include "rfid_controller.h"

// Instantiate controllers
RFIDController rfid;
RelayController relays;
AudioPlayer audio;

// Constants
constexpr uint8_t MAXIMUM_INVALID_ATTEMPTS = 13;
const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1,  3,  4,  5,  8,  12, 17,
                                                         23, 30, 38, 47, 57, 68};

// ESP32-C3 specific configuration
constexpr uint32_t RELAY1_DURATION = 1000;  // 1 second for relay 1
constexpr uint32_t RELAY2_DURATION = 1000;  // 1 second for relay 2
constexpr uint8_t RELAY1_PIN = 0;           // Relay 1 (0-indexed)
constexpr uint8_t RELAY2_PIN = 1;           // Relay 2 (0-indexed)

// State variables
uint8_t invalidAttempts = 0;
bool scanned = false;
bool impatient = false;
unsigned long relayActivatedTime = 0;
bool relayActive = false;

// ESP32-C3 specific state for sequential relay activation
enum RelayState { RELAY_IDLE, RELAY1_ACTIVE, RELAY2_PENDING, RELAY2_ACTIVE };
RelayState currentRelayState = RELAY_IDLE;

void setup() {
    Serial.begin(115200);

    // ESP32-C3 might need additional time for USB CDC to initialize
    delay(2000);

    Serial.println(F("Starting up!"));

    // Initialize controllers
    if (!rfid.begin()) {
        Serial.print(F("Didn't find PN53x board"));
        while (1)
            ;  // halt
    }

    // Print firmware version like the original
    rfid.printFirmwareVersion();

    // Initialize with default UIDs
    rfid.initializeDefaultUIDs();

    relays.begin();
    relays.setAllRelays(false);  // All relays off initially

    if (audio.begin()) {
        audio.setVolume(20);  // Adjusted to match manufacturer's default
        delay(500);
        audio.playTrack(AudioPlayer::SOUND_STARTUP);
    }

    Serial.println(F("Waiting for an ISO14443A card"));
}

void handleRelaySequence() {
    // ESP32-C3 sequential relay activation as per manufacturer
    switch (currentRelayState) {
        case RELAY_IDLE:
            // Nothing to do
            break;

        case RELAY1_ACTIVE:
            if (millis() - relayActivatedTime >= RELAY1_DURATION) {
                relays.setRelay(RELAY1_PIN, false);
                relays.setRelay(RELAY2_PIN, true);
                relayActivatedTime = millis();
                currentRelayState = RELAY2_ACTIVE;
                Serial.println(F("Relay 1 OFF, Relay 2 ON"));
            }
            break;

        case RELAY2_ACTIVE:
            if (millis() - relayActivatedTime >= RELAY2_DURATION) {
                relays.setRelay(RELAY2_PIN, false);
                currentRelayState = RELAY_IDLE;
                relayActive = false;
                Serial.println(F("Relay 2 OFF - Sequence complete"));
            }
            break;
    }
}

void activateRelays() {
    // ESP32-C3 sequential activation
    relays.setRelay(RELAY1_PIN, true);
    relayActivatedTime = millis();
    currentRelayState = RELAY1_ACTIVE;
    relayActive = true;
    Serial.println(F("Starting relay sequence - Relay 1 ON"));
}

void loop() {
    boolean success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
    uint8_t uidLength;  // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Handle relay timing
    handleRelaySequence();

    // Check if we should play the waiting sound
    if (millis() > 10000 && !impatient && !scanned) {
        audio.playTrack(AudioPlayer::SOUND_WAITING);
        impatient = true;
    }

    // Try to read a card
    success = rfid.readCard(uid, &uidLength);

    if (success) {
        scanned = true;
        Serial.println(F("Found a card!"));
        Serial.print(F("UID Length: "));
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print(F("UID Value: "));
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(" 0x");
            Serial.print(uid[i], HEX);
        }
        Serial.println("");

        bool validUID = rfid.validateUID(uid, uidLength);

        if (uidLength == 4) {
            Serial.println("4B UID");
        } else if (uidLength == 7) {
            Serial.println("7B UID");
        } else {
            Serial.print(F("Unknown UID type / length"));
        }

        if (validUID) {  // IF CORRECT UID DO THIS.
            Serial.print(F("Card match found!"));
            invalidAttempts = 0;  // RESETS THE DOUBLE DOWN DELAY
            audio.playTrack(AudioPlayer::SOUND_ACCEPTED);

            // Activate relays based on platform
            activateRelays();

        } else {
            Serial.print(F("Unauthorised card"));
            if (invalidAttempts == 0) {
                audio.playTrack(AudioPlayer::SOUND_DENIED_1);
            } else if (invalidAttempts == 1) {
                audio.playTrack(AudioPlayer::SOUND_DENIED_2);
            } else {
                audio.playTrack(AudioPlayer::SOUND_DENIED_3);
            }
            delay(3000);
            delay(invalidDelays[invalidAttempts] * 1000);
            if (invalidAttempts <
                MAXIMUM_INVALID_ATTEMPTS - 1)  // THIS DOUBLES THE DELAYTIME BETWEEN EACH WRONG UID
                                               // SCAN,(BRUTEFORCE DETERRANT)
            {
                invalidAttempts++;
            }
        }
    }
}

#endif
