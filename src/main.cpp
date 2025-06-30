#include <Arduino.h>
#include <access_service.h>
#include "esp_log.h"
#include "exception_handler.h"

static const char* TAG = "Main";

void setup() {
    // Set global exception handler first
    setupGlobalExceptionHandler();

    Serial.begin(115200);
    delay(1000);

    ESP_LOGE(TAG, "Chelonian Access Service");
    ESP_LOGE(TAG, "Version 1.0.0");
    ESP_LOGE(TAG, "Copyright (C) 2023 Derek Molloy");
    ESP_LOGE(TAG, "Licensed under the MIT License");
    ESP_LOGE(TAG, "Starting up!");

    // Call the setup function for the access service
    delay(1000);
    accessServiceSetup();
}

void loop() {
    // Call the main service loop
    accessServiceLoop();
}
