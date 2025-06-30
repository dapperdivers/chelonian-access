# PlatformIO Unit Testing Best Practices

## Best Practices for Project Structure and Unit Testing

PlatformIO documentation highlights several key best practices for unit testing, particularly relevant to the "multiple main methods" conflict:

1.  **Tests should be independent and repeatable.** PlatformIO isolates tests by running each as a separate application.
2.  **Tests should not depend on the main application source code.** Each test is an independent application and should contain its own `main()` function (or `setup()/loop()` for Arduino, `app_main()` for Espressif IoT Development Framework). Linking the main application source code from `src_dir` with a test suite code will lead to multiple compilation errors. Hence, the `Shared Code` is disabled by default.
3.  **Tests should be well organized and reflect the structure of the tested code.** PlatformIO allows organizing tests using nested folders. Any folder prefixed with `test_` is considered an independent test/application.

Crucially, point #2 directly addresses the user's problem: "Linking the main application source code from `src_dir` with a test suite code will lead to multiple compilation errors." This confirms that the user's proposed solution of excluding `src/main.cpp` from the test build is in line with PlatformIO's recommended practices.



## Analysis of the Proposed Solution

The user's proposed solution directly addresses the "multiple main methods" conflict and aligns with PlatformIO's recommended best practices for unit testing:

1.  **Reverting `src/main.cpp` to its original state**: This is crucial because, as the user noted, the application build (env:esp32c3) relies on a `main` function being present in `src/main.cpp`. By reverting it, the application build remains functional.

2.  **Excluding `src/main.cpp` from the native test build using `src_filter = -<src/main.cpp>`**: This is the core of the solution and is explicitly supported by PlatformIO's build system. The documentation states that "Linking the main application source code from `src_dir` with a test suite code will lead to multiple compilation errors." By using `src_filter`, the `src/main.cpp` file is effectively removed from the compilation scope when building the native unit tests, preventing the `main()` function conflict.

This approach ensures that:
*   The application build proceeds without issues, as `src/main.cpp` is included.
*   The unit test build proceeds without issues, as `src/main.cpp` (and its `main()` function) is excluded, allowing the Unity test runner's `main()` to be the sole entry point.

Therefore, the user's proposed solution is **correct** and adheres to the best practices outlined in the PlatformIO documentation for managing separate application and test builds, especially concerning the `main()` function conflict.



## Alternative Approaches and Best Practices

While the user's proposed solution using `src_filter = -<src/main.cpp>` is effective and correct for resolving the immediate "multiple main methods" conflict, PlatformIO documentation suggests a more robust and recommended approach for managing shared code between the main application and unit tests:

### 1. Recommended Approach: Utilizing `lib_dir` for Shared Components

PlatformIO strongly recommends splitting shared source code into multiple components and placing them into the `lib_dir` (project’s private libraries and components). The Library Dependency Finder (LDF) will automatically find and include these libraries in the build process for both the main application and the tests.

**How to implement this approach:**

*   **Identify shared code**: Any functions, classes, or modules that are used by both your main ESP32 application (`src/main.cpp`) and your unit tests (`test/test_native/test_relay/main.cpp`) should be considered shared code.
*   **Move shared code to `lib_dir`**: Create subdirectories within the `lib` folder for each logical component. For example, if you have a `relay_control` module, you would place its `.h` and `.cpp` files in `lib/relay_control/`.
*   **Include in `main.cpp` and test files**: In your `src/main.cpp` and your test files, you would then include these components using `#include <ComponentName.h>` (e.g., `#include <relay_control.h>`).
*   **Remove `test_build_src = yes`**: With this approach, you would typically remove `test_build_src = yes` from your `platformio.ini` for the test environment, as the shared code is now managed through `lib_dir` and not directly from `src_dir`.

**Benefits of this approach:**

*   **Clear separation of concerns**: Promotes a modular design where application logic is clearly separated from testing concerns.
*   **Improved reusability**: Components in `lib_dir` are easily reusable across different parts of your project or even in other projects.
*   **Simplified build process**: The LDF handles dependencies automatically, reducing the need for manual `src_filter` rules for shared code.
*   **Adherence to PlatformIO philosophy**: This aligns with PlatformIO's intended way of structuring projects for embedded development and testing.

### 2. User's Proposed Solution: Using `src_filter` (Effective for `main.cpp` Exclusion)

The user's solution of using `src_filter = -<src/main.cpp>` in the `[env:native]` section of `platformio.ini` is a perfectly valid and effective way to specifically exclude `src/main.cpp` from the test build. This is particularly useful when `src/main.cpp` contains `setup()` and `loop()` or `app_main()` functions that are specific to the application and should not be compiled with the test runner's `main()`.

**When to use this approach:**

*   When the `src/main.cpp` file primarily contains application-specific entry points (`setup()`, `loop()`, `app_main()`) and very little, if any, reusable logic that needs to be tested directly.
*   When you want a quick and direct way to resolve the `multiple main methods` error without refactoring your entire codebase into libraries.

**Considerations:**

*   If `src/main.cpp` starts accumulating significant amounts of reusable logic that *should* be unit tested, it's a strong indicator that those parts should be refactored into separate files and ideally moved to the `lib_dir`.

### Summary of Best Practices for ESP32 Application with Native Unit Tests:

*   **Isolate `main()` functions**: Ensure that your application build and your test builds each have a single, distinct `main()` (or equivalent entry point like `setup()/loop()`).
*   **Use `lib_dir` for shared code**: For any code that is common to both your application and your tests, refactor it into private libraries within the `lib` directory. This is the most robust and recommended approach for long-term maintainability and reusability.
*   **Employ `src_filter` for specific exclusions**: If you have files like `src/main.cpp` that are purely application entry points and cause conflicts with test runners, `src_filter` is an excellent tool for targeted exclusion.
*   **Conditional compilation (`#ifdef UNIT_TEST`)**: While the user initially used this, it's generally less preferred for managing build inclusions/exclusions in PlatformIO compared to `platformio.ini` directives like `src_filter` or proper `lib_dir` structuring. However, it can still be useful for small, isolated conditional logic within a single file.

By combining these strategies, you can maintain a clean, efficient, and testable PlatformIO project for your ESP32 applications.
