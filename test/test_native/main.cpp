#include <unity.h>

// Optional: Define setup and loop for Arduino compatibility if needed
// void setup() {
//     // Not used in native tests, but required for Arduino framework compatibility
// }

// void loop() {
//     // Not used in native tests, but required for Arduino framework compatibility
// }

// This is the main entry point for the native test executable
int main(int argc, char** argv) {
    UNITY_BEGIN();  // Initialize Unity framework
    // Individual test files will be compiled and linked.
    // Unity's test runner will discover and execute tests marked with RUN_TEST.
    return UNITY_END();  // Finalize Unity framework and return test result
}
