#include <Arduino.h>
#include <access_service.h>
#include <cstdlib>
#include <exception>
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
    esp_log_level_set("*", ESP_LOG_VERBOSE);  // Explicitly set log level for all tags to VERBOSE
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);  // Explicitly set log level for "Main" tag
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
