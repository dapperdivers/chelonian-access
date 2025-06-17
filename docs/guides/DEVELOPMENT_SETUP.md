# Development Environment Setup Guide

This guide will help you set up your development environment for working on the Chelonian Access project. We use `mise` to manage our development tools and dependencies consistently across all development environments.

## Prerequisites

- Linux-based operating system (Ubuntu/Debian recommended)
- Git (for version control)
- curl (for downloading tools)
- Basic development tools (build-essential, gcc, g++, make)
- Python 3.7 or higher (for PlatformIO)

## Step 1: Install mise (Tool Version Manager)

1. Install mise:

   ```bash
   curl https://mise.run | sh
   ```

2. Add mise to your shell (choose one based on your shell):

   For zsh (add to ~/.zshrc):

   ```bash
   echo 'eval "$(~/.local/bin/mise activate zsh)"' >> ~/.zshrc
   source ~/.zshrc
   ```

   For bash (add to ~/.bashrc):

   ```bash
   echo 'eval "$(~/.local/bin/mise activate bash)"' >> ~/.bashrc
   source ~/.bashrc
   ```

## Step 2: Install Basic System Dependencies

If you haven't already, install the required system packages:

```bash
sudo apt update
sudo apt install -y build-essential gcc g++ make python3 python3-pip
```

## Step 3: Clone and Set Up the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/chelonian-access.git
   cd chelonian-access
   ```

2. Let mise install all required tools and dependencies:

   ```bash
   mise setup
   ```

   This will automatically install:
   - PlatformIO (for embedded development)
   - clang-format (v17.0.6) - Code formatter
   - clang-tidy - Static analyzer
   - cppcheck - Static analysis tool
   - uncrustify - Code beautifier
   - OCLint - Static code analysis
   - pre-commit hooks - Automated code quality checks

## Step 4: Initialize Pre-commit Hooks

The pre-commit hooks are automatically installed during setup, but you can manually install them if needed:

```bash
pre-commit install
```

## Step 5: Verify Installation

Run the following commands to verify your setup:

1. Verify all basic development tools:

   ```bash
   mise run verify-basic-tools
   ```

2. Verify all development tools and linters:

   ```bash
   mise run verify-dev-tools
   ```

3. Test the linters:

   ```bash
   mise run lint
   ```

4. Verify PlatformIO installation:

   ```bash
   mise run verify-pio
   ```

## Common Tasks

- Run all linters:

  ```bash
  mise run lint
  ```

- Build the project:

  ```bash
  mise run build
  ```

- Run tests:

  ```bash
  mise run test
  ```

- Install new dependencies:

  ```bash
  mise run install-deps
  ```

- Update OCLint:

  ```bash
  mise run install-oclint
  ```

## Troubleshooting

### Basic Tools Issues

If you're missing basic development tools:

```bash
mise run install-basic-tools
```

### Development Tool Issues

To reinstall all development tools:

```bash
mise run install-deps && mise run install-oclint
```

To verify what's installed and what's missing:

```bash
mise run verify-dev-tools
```

### PlatformIO Issues

If you encounter PlatformIO-related issues:

```bash
mise run repair-pio
```

### Pre-commit Hook Issues

If pre-commit hooks aren't running:

```bash
pre-commit uninstall && pre-commit install
```

## Project Structure

Our key development configuration files:

- `.mise.toml` - Development environment configuration
- `.pre-commit-config.yaml` - Linting and code quality checks configuration
- `.clang-format` - C++ code formatting rules
- `.clang-tidy` - Static analyzer configuration
- `platformio.ini` - PlatformIO project configuration

## Next Steps

- Review the [Pre-commit Setup](development/PRE_COMMIT_SETUP.md) for detailed information about our code quality tools
- Check out the [Project Overview](docs/PROJECT_OVERVIEW.md) to understand the system architecture
- Review our [Current Features](docs/CURRENT_FEATURES.md) documentation

## Need Help?

If you encounter any issues not covered in this guide:

1. Check the existing GitHub issues
2. Review our documentation in the `docs/` directory
3. Create a new issue with detailed information about your problem
