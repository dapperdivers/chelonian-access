# Building for a "Native" Environment in PlatformIO

This guide provides comprehensive instructions and best practices for configuring your PlatformIO project to build and run in a "native" environment. A native environment typically refers to your host machine (e.g., Windows, macOS, Linux) and is commonly used for faster unit testing, integration testing, or even developing parts of your application that don't require direct hardware interaction.

## 1. Understanding the "Native" Environment in PlatformIO

PlatformIO's strength lies in its ability to manage various development platforms, including embedded systems like ESP32 and native desktop environments. When you configure a "native" environment, you are essentially telling PlatformIO to compile your code using your host machine's compiler (e.g., GCC) rather than a cross-compiler for an embedded target. This allows for quicker compilation and execution of tests or applications that don't rely on specific hardware peripherals.

## 2. Configuring `platformio.ini` for a Native Build

To add a native build configuration, you need to define a new environment in your `platformio.ini` file. This file is the central configuration hub for your PlatformIO project. Here's a typical setup for a native environment:

```ini
[env:native]
platform = native
build_flags =
  -std=c++11
  -Wall
  -Wextra
  -Werror
lib_deps =
  # Add any host-compatible libraries here
  # For example, if using Unity for testing:
  # https://github.com/ThrowTheSwitch/Unity.git

# Optional: For unit testing, especially with Unity
# test_framework = unity
# test_build_src = yes ; Only if you want to include src/ files in test build
# src_filter = +<*> -<src/main.cpp> ; Example to exclude main.cpp from src
```

Let's break down the key components of this configuration:

*   **`[env:native]`**: This defines a new build environment named `native`. You can choose any descriptive name for your environment.

*   **`platform = native`**: This is the crucial line that tells PlatformIO to use the native development platform. This will configure the build system to use your host machine's compiler and tools.

*   **`build_flags`**: These are compiler flags that will be applied during the build process. Common flags include:
    *   `-std=c++11` (or `c++14`, `c++17`, etc.): Specifies the C++ standard to use.
    *   `-Wall`, `-Wextra`, `-Werror`: Enable various warnings and treat warnings as errors, which is good practice for robust code.

*   **`lib_deps`**: This section is used to declare external library dependencies. For a native environment, you would typically include libraries that are compatible with your host system. For example, if you are setting up a native environment for unit testing with Unity, you would specify the Unity library here.

*   **`test_framework` (Optional)**: If this native environment is specifically for unit testing, you can specify the testing framework you are using (e.g., `unity`). PlatformIO has built-in support for several testing frameworks.

*   **`test_build_src` (Optional, use with caution)**: Setting this to `yes` will include all source files from your `src` directory in the test build. As discussed in the previous problem, this can lead to "multiple main methods" errors if your `src/main.cpp` also contains a `main()` function. Use this only if you intend to test code directly from `src` and have managed `main()` function conflicts (e.g., through conditional compilation or `src_filter`).

*   **`src_filter` (Optional, highly recommended for `main()` conflicts)**: This powerful option allows you to explicitly include or exclude source files from the build. It's particularly useful for resolving `main()` function conflicts. The syntax `+<*>` includes all files, and `-<src/main.cpp>` then excludes `src/main.cpp`. This is the recommended way to handle the "multiple main methods" issue when `test_build_src = yes` is used, or when you have multiple entry points for different build targets.



## 3. Source File Management for Native Builds

Managing your source files effectively is paramount when working with native builds, especially to avoid conflicts like the "multiple main methods" error you encountered. The key principle is to ensure that each build target (your main application and your native tests/applications) has a single, unambiguous entry point (`main()` function) and access to only the necessary source code.

### 3.1 Handling `main()` Functions

In C/C++, every executable program requires a `main()` function as its entry point. When you have both an embedded application (e.g., for ESP32) and a native application (for testing), both will naturally define their own `main()` functions. The conflict arises when the build system attempts to link both `main()` functions into a single executable.

To prevent this, you have a few primary strategies:

*   **Exclusive `main()` for each build**: The most straightforward approach is to ensure that the `main()` function for your embedded application (`src/main.cpp`) is *only* compiled when building for the embedded target, and the `main()` function for your native tests (`test/test_native/test_relay/main.cpp` or similar) is *only* compiled when building for the native target.

*   **Using `src_filter` for exclusion**: As you correctly identified and implemented, `src_filter` in `platformio.ini` is an effective way to exclude specific files from a build environment. For your native test environment, adding `src_filter = -<src/main.cpp>` ensures that your main application's `main.cpp` is not included in the native build, thus preventing the conflict with your test runner's `main()`.

*   **Conditional Compilation (`#ifdef`)**: While less preferred for managing entire file inclusions/exclusions, conditional compilation can be used within a single file if you have small, specific blocks of code (including a `main()` placeholder) that should only be active under certain build conditions. For example:

    ```cpp
    #ifndef UNIT_TEST
    // This main() is for the application build
    int main() {
      // Application setup and loop
      return 0;
    }
    #endif

    // In your test file (e.g., test/test_native/test_relay/main.cpp)
    // This main() is for the unit test build
    int main() {
      UNITY_BEGIN();
      // Run tests
      UNITY_END();
      return 0;
    }
    ```
    However, relying heavily on `#ifdef` for large-scale file management can make your `platformio.ini` less clear and your codebase more complex. `src_filter` is generally cleaner for file-level control.

### 3.2 Managing Shared Code (`lib_dir` vs. `src_dir`)

Often, your application code contains functions, classes, or modules that you want to unit test on the native environment. This shared code needs to be accessible to both your embedded application build and your native test build.

PlatformIO offers two main ways to handle shared code, with a strong recommendation for one over the other:

#### 3.2.1 Recommended: Using the `lib_dir` (Project Private Libraries)

PlatformIO's documentation strongly advocates for placing shared code in the `lib` directory (e.g., `your_project/lib/your_component/`). This directory is designed for project-specific libraries that are automatically discovered and linked by PlatformIO's Library Dependency Finder (LDF) when included in your source files.

**Advantages of `lib_dir`:**

*   **Modularity and Reusability**: Promotes breaking down your application into independent, reusable components.
*   **Automatic Dependency Management**: The LDF handles the inclusion of these libraries in both your application and test builds, reducing manual configuration.
*   **Clear Separation**: Clearly distinguishes reusable components from application-specific entry points.
*   **Best Practice**: Aligns with the standard way of structuring PlatformIO projects for maintainability and scalability.

**How to use `lib_dir`:**

1.  **Create a component directory**: Inside your project's `lib` folder, create a new directory for your shared component (e.g., `lib/my_sensor_driver/`).
2.  **Place source files**: Move the `.h` and `.cpp` files for your shared code into this new directory.
3.  **Include in your code**: In `src/main.cpp` and your native test files, include the component's header: `#include <my_sensor_driver.h>`.

#### 3.2.2 Less Recommended: Including `src_dir` in Test Builds (`test_build_src = yes`)

While possible, directly including the `src` directory in your test build using `test_build_src = yes` is generally **not recommended** by PlatformIO, especially if your `src` directory contains application-specific `main()` functions or other code that shouldn't be part of a unit test executable.

**When `test_build_src = yes` might be considered (with caveats):**

*   For very small projects where `src/main.cpp` is minimal and you're using `src_filter` or conditional compilation to manage the `main()` conflict.
*   When you are testing an independent library that happens to reside in `src` and doesn't have a conflicting `main()` function.

**Disadvantages of `test_build_src = yes`:**

*   **`main()` conflicts**: As you experienced, this is the primary cause of the "multiple main methods" error.
*   **Unnecessary compilation**: Can lead to compiling code that is irrelevant to your unit tests, increasing build times.
*   **Less modular**: Discourages breaking down your project into reusable libraries.

**If you must use `test_build_src = yes`:**

*   **Always combine with `src_filter`**: Explicitly exclude `src/main.cpp` (or any other conflicting files) using `src_filter = +<*> -<src/main.cpp>`.
*   **Be mindful of dependencies**: Ensure that any code in `src` that you are including for testing does not have dependencies that are incompatible with your native environment.

In summary, for robust and maintainable PlatformIO projects with native builds, prioritize moving shared, testable code into the `lib_dir`. Use `src_filter` as a precise tool for excluding specific files (like `src/main.cpp`) from certain build environments to prevent `main()` conflicts.



## 4. Comprehensive Example and Summary of Best Practices

Let's put it all together with a practical example demonstrating a PlatformIO project structure and `platformio.ini` configuration that supports both an ESP32 application and native unit tests.

### Example Project Structure

```
my_platformio_project/
├── .pio/
├── lib/
│   └── my_shared_module/
│       ├── my_shared_module.h
│       └── my_shared_module.cpp
├── src/
│   └── main.cpp
├── test/
│   └── test_native/
│       ├── test_my_shared_module.cpp
│       └── main.cpp  # Unity test runner's main()
├── platformio.ini
└── README.md
```

### Example `platformio.ini`

```ini
[env:esp32c3_app]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
build_flags =
  -DAPP_BUILD
lib_deps =
  # Your application-specific libraries

[env:native_test]
platform = native
build_flags =
  -std=c++11
  -Wall
  -Wextra
  -Werror
lib_deps =
  # Unity Test Framework
  https://github.com/ThrowTheSwitch/Unity.git

test_framework = unity

# Crucial for preventing 'multiple main methods' conflict
# Excludes src/main.cpp from the native test build
src_filter = +<*> -<src/main.cpp>

# Optional: If you have other files in src/ that are NOT main.cpp
# and you want to include them in the test build, you can use:
# test_build_src = yes
# But remember to keep src_filter to exclude main.cpp
```

### `src/main.cpp` (for ESP32 Application)

```cpp
#include <Arduino.h>
#include <my_shared_module.h>

// This main() is for the ESP32 application build
// It will NOT be compiled for the native_test environment due to src_filter
#ifndef UNIT_TEST // A common guard, though src_filter is more robust for file exclusion
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Application Started");
  MySharedModule::init();
}

void loop() {
  MySharedModule::doSomething();
  delay(1000);
}
#endif

// A placeholder main for some embedded frameworks that require it,
// but often setup()/loop() are sufficient for Arduino framework.
// If your framework truly requires a main(), ensure it's guarded.
// int main() { setup(); while(true) { loop(); } return 0; }
```

### `lib/my_shared_module/my_shared_module.h`

```cpp
#ifndef MY_SHARED_MODULE_H
#define MY_SHARED_MODULE_H

#include <Arduino.h> // Can be conditionally included for native build if needed

namespace MySharedModule {
  void init();
  void doSomething();
  int add(int a, int b);
}

#endif // MY_SHARED_MODULE_H
```

### `lib/my_shared_module/my_shared_module.cpp`

```cpp
#include "my_shared_module.h"
#include <iostream> // For native build output

namespace MySharedModule {
  void init() {
    #ifdef ARDUINO
      Serial.println("MySharedModule initialized on ESP32");
    #else
      std::cout << "MySharedModule initialized on Native" << std::endl;
    #endif
  }

  void doSomething() {
    #ifdef ARDUINO
      Serial.println("MySharedModule doing something on ESP32");
    #else
      std::cout << "MySharedModule doing something on Native" << std::endl;
    #endif
  }

  int add(int a, int b) {
    return a + b;
  }
}
```

### `test/test_native/test_my_shared_module.cpp`

```cpp
#include <unity.h>
#include <my_shared_module.h>

void test_add_function(void) {
    TEST_ASSERT_EQUAL(5, MySharedModule::add(2, 3));
    TEST_ASSERT_EQUAL(0, MySharedModule::add(-1, 1));
}

// This main() is for the native test build
// It will be the entry point when running tests in the native environment
int main() {
    UNITY_BEGIN();
    RUN_TEST(test_add_function);
    return UNITY_END();
}
```

### Summary of Best Practices for Native Builds:

1.  **Dedicated Native Environment**: Always define a separate environment in `platformio.ini` (e.g., `[env:native_test]`) with `platform = native`.
2.  **Isolate `main()` Functions**: Ensure that your application's `main()` (or `setup()/loop()`) and your test runner's `main()` are never compiled together. The `src_filter = -<src/main.cpp>` directive is highly effective for this.
3.  **Leverage `lib_dir` for Shared Code**: Move all reusable code that needs to be accessed by both your application and your tests into the `lib` directory as private libraries. This promotes modularity, reusability, and simplifies dependency management.
4.  **Use Appropriate Build Flags**: Configure `build_flags` for your native environment to enable strict warnings (`-Wall`, `-Wextra`, `-Werror`) and specify the C++ standard (`-std=c++11` or higher).
5.  **Specify Test Framework**: If the native build is for testing, explicitly set `test_framework` (e.g., `unity`) in `platformio.ini`.
6.  **Conditional Includes/Compilation**: Use `#ifdef` or similar preprocessor directives within shared code (e.g., in `my_shared_module.cpp`) to handle platform-specific differences (e.g., `Serial.print` vs. `std::cout`).
7.  **Clear Test Structure**: Organize your tests logically within the `test` directory, following PlatformIO's conventions (e.g., `test/test_native/`).

By following these guidelines, you can effectively integrate native builds into your PlatformIO workflow, significantly speeding up your development and testing cycles for embedded projects.
