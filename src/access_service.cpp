#include "access_service.h"

#ifdef UNIT_TEST
#include "mock_arduino.h"
#else
#include <Arduino.h>
#endif
// Instantiate controllers
RFIDController rfid;
RelayController relays;
AudioPlayer audio;

// State variables
enum RelayState currentRelayState = RELAY_IDLE;
uint8_t invalidAttempts = 0;
bool scanned = false;
bool impatient = false;
unsigned long relayActivatedTime = 0;
bool relayActive = false;

const uint8_t invalidDelays[MAXIMUM_INVALID_ATTEMPTS] = {1,  3,  4,  5,  8,  12, 17,
                                                         23, 30, 38, 47, 57, 68};

void accessServiceSetup() {
    if (!rfid.begin()) {
        Serial.print(F("Didn't find PN53x board"));
        while (1)
            ;
    }
    rfid.printFirmwareVersion();
    rfid.initializeDefaultUIDs();
    relays.begin();
    relays.setAllRelays(false);
    if (audio.begin()) {
        audio.setVolume(20);
        delay(500);
        audio.playTrack(AudioPlayer::SOUND_STARTUP);
    }
    Serial.println(F("Waiting for an ISO14443A card"));
}

void handleRelaySequence() {
    switch (currentRelayState) {
        case RELAY_IDLE:
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
    relays.setRelay(RELAY1_PIN, true);
    relayActivatedTime = millis();
    currentRelayState = RELAY1_ACTIVE;
    relayActive = true;
    Serial.println(F("Starting relay sequence - Relay 1 ON"));
}

void accessServiceLoop() {
    boolean success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t uidLength;
    handleRelaySequence();
    if (millis() > 10000 && !impatient && !scanned) {
        audio.playTrack(AudioPlayer::SOUND_WAITING);
        impatient = true;
    }
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
        if (validUID) {
            Serial.print(F("Card match found!"));
            invalidAttempts = 0;
            audio.playTrack(AudioPlayer::SOUND_ACCEPTED);
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
            if (invalidAttempts < MAXIMUM_INVALID_ATTEMPTS - 1) {
                invalidAttempts++;
            }
        }
    }
}
