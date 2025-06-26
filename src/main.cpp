#ifndef UNIT_TEST

#include <Arduino.h>
#include "access_service.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println(F("Starting up!"));

    // Call the setup function for the access service
    accessServiceSetup();
}

void loop() {
    // Call the main service loop
    accessServiceLoop();
}

#endif
