#ifndef UNIT_TEST

#include <Arduino.h>
#include <cstdlib>
#include <exception>
#include "access_service.h"

void globalExceptionHandler() {
    Serial.println(F("\n\n=== UNHANDLED EXCEPTION ==="));

    // Print exception info if available
    if (std::current_exception()) {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& e) {
            Serial.print(F("Exception Type: "));
            Serial.println(e.what());
        } catch (...) {
            Serial.println(F("Unknown exception type"));
        }
    }

// Print stack trace if available (ESP32 specific)
#ifdef ESP32
    Serial.println(F("\nStack Trace:"));
#endif

    Serial.println(F("\nSystem will restart in 5 seconds..."));
    delay(5000);
    ESP.restart();
}

void setup() {
    // Set global exception handler first
    std::set_terminate(globalExceptionHandler);

    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println(F("Chelonian Access Service"));
    Serial.println(F("Version 1.0.0"));
    Serial.println(F("Copyright (C) 2023 Derek Molloy"));
    Serial.println(F("Licensed under the MIT License"));
    Serial.println(F("Starting up!"));

    // Call the setup function for the access service
    accessServiceSetup();
}

void loop() {
    // Call the main service loop
    accessServiceLoop();
}
#endif
