#ifndef UNIT_TEST

#include <Arduino.h>
#include <cstdlib>
#include <exception>
#include "access_service.h"
#include "esp_log.h"

static const char* TAG = "Main";

void globalExceptionHandler() {
    ESP_LOGE(TAG, "\n\n=== UNHANDLED EXCEPTION ===");

    // Print exception info if available
    if (std::current_exception()) {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& e) {
            ESP_LOGE(TAG, "Exception Type: %s", e.what());
        } catch (...) {
            ESP_LOGE(TAG, "Unknown exception type");
        }
    }

// Print stack trace if available (ESP32 specific)
#ifdef ESP32
    ESP_LOGE(TAG, "\nStack Trace:");
#endif

    ESP_LOGE(TAG, "\nSystem will restart in 5 seconds...");
    delay(5000);
    ESP.restart();
}

void setup() {
    // Set global exception handler first
    std::set_terminate(globalExceptionHandler);

    Serial.begin(115200);
    delay(2000);
    ESP_LOGI(TAG, "Chelonian Access Service");
    ESP_LOGI(TAG, "Version 1.0.0");
    ESP_LOGI(TAG, "Copyright (C) 2023 Derek Molloy");
    ESP_LOGI(TAG, "Licensed under the MIT License");
    ESP_LOGI(TAG, "Starting up!");

    // Call the setup function for the access service
    accessServiceSetup();
}

void loop() {
    // Call the main service loop
    accessServiceLoop();
}
#endif
