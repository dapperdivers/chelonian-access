# Clang-tidy Implementation Guide

## Overview

This document provides the implementation details for integrating clang-tidy static analysis into the Chelonian Access project CI/CD pipeline.

## Implementation Steps

### 1. Create .clang-tidy Configuration File

Create a `.clang-tidy` file in the project root with the following content:

```yaml
# Clang-tidy configuration for Chelonian Access Project
Checks: >
  -*,
  bugprone-*,
  -bugprone-easily-swappable-parameters,
  cppcoreguidelines-*,
  -cppcoreguidelines-avoid-magic-numbers,
  -cppcoreguidelines-non-private-member-variables-in-classes,
  -cppcoreguidelines-pro-bounds-array-to-pointer-decay,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  modernize-*,
  -modernize-use-trailing-return-type,
  performance-*,
  readability-*,
  -readability-magic-numbers,
  -readability-uppercase-literal-suffix,
  misc-*

WarningsAsErrors: ''
HeaderFilterRegex: '.*\.(h|hpp)$'
AnalyzeTemporaryDtors: false

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.FunctionCase
    value: camelBack
  - key: readability-identifier-naming.VariableCase
    value: camelBack
  - key: readability-identifier-naming.ConstantCase
    value: UPPER_CASE
  - key: readability-identifier-naming.PrivateMemberPrefix
    value: 'm_'
  - key: readability-identifier-naming.ProtectedMemberPrefix
    value: 'm_'
  - key: cppcoreguidelines-explicit-virtual-functions.IgnoreDestructors
    value: '1'
  - key: modernize-use-override.IgnoreDestructors
    value: '1'
  - key: modernize-pass-by-value.IncludeStyle
    value: google
```

### 2. Update GitHub Actions Workflow

Modify `.github/workflows/platformio-tests.yml` to add clang-tidy analysis. Add the following job after the existing test job:

```yaml
  static-analysis:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.x'
    
    - name: Cache PlatformIO
      uses: actions/cache@v3
      with:
        path: |
          ~/.platformio
          .pio
        key: ${{ runner.os }}-pio-${{ hashFiles('**/platformio.ini') }}
        restore-keys: |
          ${{ runner.os }}-pio-
    
    - name: Install dependencies
      run: |
        python -m pip install --upgrade pip
        pip install platformio
        # Install clang-tidy
        sudo apt-get update
        sudo apt-get install -y clang-tidy
    
    - name: Generate compile_commands.json
      run: |
        # Generate compilation database for native environment
        pio run -t compiledb -e native
        # Move it to project root for clang-tidy
        cp .pio/build/native/compile_commands.json .
    
    - name: Run clang-tidy
      run: |
        # Create results directory
        mkdir -p clang-tidy-results
        
        # Run clang-tidy on all source files and convert to SARIF
        find src include -name "*.cpp" -o -name "*.h" | \
        xargs clang-tidy -p . 2>&1 | \
        tee clang-tidy-results/output.txt | \
        clang-tidy-sarif | \
        tee clang-tidy-results/results.sarif | \
        sarif-fmt
        
        # Also export fixes for potential auto-fixing
        find src include -name "*.cpp" -o -name "*.h" | \
        xargs clang-tidy -p . --export-fixes=clang-tidy-results/fixes.yaml 2>&1 > /dev/null || true
        
        # Check if there were any warnings
        if grep -q "warning:" clang-tidy-results/output.txt; then
          echo "::warning::Clang-tidy found issues. See artifacts for details."
        fi
    
    - name: Generate HTML report
      if: always()
      run: |
        # Create a simple HTML report
        cat > clang-tidy-results/report.html << 'EOF'
        <!DOCTYPE html>
        <html>
        <head>
          <title>Clang-tidy Report</title>
          <style>
            body { font-family: monospace; margin: 20px; }
            .warning { color: #856404; background-color: #fff3cd; padding: 5px; margin: 5px 0; }
            .error { color: #721c24; background-color: #f8d7da; padding: 5px; margin: 5px 0; }
            pre { background-color: #f5f5f5; padding: 10px; overflow-x: auto; }
          </style>
        </head>
        <body>
          <h1>Clang-tidy Analysis Report</h1>
          <pre>
        EOF
        
        cat clang-tidy-results/output.txt >> clang-tidy-results/report.html
        
        cat >> clang-tidy-results/report.html << 'EOF'
          </pre>
        </body>
        </html>
        EOF
    
    - name: Upload clang-tidy results
      uses: actions/upload-artifact@v4
      if: always()
      with:
        name: clang-tidy-results
        path: clang-tidy-results/
        retention-days: 30
    
    - name: Upload SARIF to GitHub Security
      uses: github/codeql-action/upload-sarif@v3
      if: always() && github.event_name == 'push'
      with:
        sarif_file: clang-tidy-results/results.sarif
        category: clang-tidy
```

### 3. Create Helper Script (Optional)

Create `scripts/run-clang-tidy.sh` for local development:

```bash
#!/bin/bash
# Run clang-tidy locally

# Generate compile_commands.json
echo "Generating compile_commands.json..."
pio run -t compiledb -e native
cp .pio/build/native/compile_commands.json .

# Run clang-tidy
echo "Running clang-tidy..."
find src include -name "*.cpp" -o -name "*.h" | \
xargs clang-tidy -p . --fix

echo "Done! Check git diff for automatic fixes."
```

Make it executable:
```bash
chmod +x scripts/run-clang-tidy.sh
```

## Configuration Explanation

### Enabled Checks

- **bugprone-*** : Detects common bug patterns
- **cppcoreguidelines-*** : Enforces C++ Core Guidelines
- **modernize-*** : Suggests modern C++ idioms
- **performance-*** : Identifies performance improvements
- **readability-*** : Improves code readability
- **misc-*** : Various useful checks

### Disabled Checks

Some checks are disabled for embedded/Arduino compatibility:
- Magic numbers (common in hardware programming)
- Array-to-pointer decay (needed for C-style APIs)
- Pointer arithmetic (common in embedded)

## Integration Benefits

1. **Automatic Detection**: Finds bugs before runtime
2. **Code Quality**: Enforces consistent style
3. **Performance**: Identifies optimization opportunities
4. **Learning Tool**: Teaches modern C++ practices
5. **CI Integration**: Runs automatically on every push
6. **GitHub Security Integration**: Results appear in the Security tab via SARIF

## Usage

### In CI/CD
The analysis runs automatically on every push and pull request.

### Locally
```bash
./scripts/run-clang-tidy.sh
```

### With Auto-fix
```bash
clang-tidy -p . --fix src/*.cpp include/*.h
```

## Suppressing False Positives

For specific false positives, use:
```cpp
// NOLINTNEXTLINE(check-name)
problematic_code();

// Or for a specific line
problematic_code(); // NOLINT(check-name)
```

## Next Steps

1. Review and merge the implementation
2. Fix any legitimate issues found
3. Add suppressions for false positives
4. Consider adding more strict checks over time
5. Integrate with IDE (VS Code has excellent clang-tidy support)

## GitHub Security Integration

The workflow now includes SARIF (Static Analysis Results Interchange Format) integration:

1. **clang-tidy-sarif**: Converts clang-tidy output to SARIF format
2. **sarif-fmt**: Pretty-prints SARIF for console output
3. **GitHub Code Scanning**: Uploads SARIF to GitHub Security tab

Results will appear in:
- **Pull Requests**: Inline annotations on changed code
- **Security Tab**: Full list of code scanning alerts
- **Artifacts**: Downloadable reports for detailed analysis

### Viewing Results

After pushing code:
1. Go to the **Security** tab in your repository
2. Click on **Code scanning alerts**
3. Filter by tool: `clang-tidy`
4. Review and manage alerts