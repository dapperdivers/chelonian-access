#ifndef UNIT_TEST

#include <Arduino.h>
#include "access_service.h"
#include "audio_controller.h"
#include "relay_controller.h"
#include "rfid_controller.h"

RFIDController rfid;
RelayController relays;
AudioController audio;

AccessService accessService(rfid, relays, audio);

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
        audio.playTrack(AudioController::SOUND_STARTUP);
    }

    Serial.println(F("Waiting for an ISO14443A card"));
}

void loop() {
    accessService.loop();
}
#endif
