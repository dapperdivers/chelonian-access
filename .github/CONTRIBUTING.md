# Contributing to Chelonian Access

Thank you for your interest in contributing to the Chelonian Access project! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- A clear and descriptive title
- Steps to reproduce the issue
- Expected behavior vs actual behavior
- Hardware configuration (Pro Micro version, modules used)
- PlatformIO configuration
- Any relevant logs or error messages

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

- A clear and descriptive title
- Step-by-step description of the suggested enhancement
- Explain why this enhancement would be useful
- List any additional hardware requirements

### Pull Requests

1. Fork the repository and create your branch from `main`
2. Follow the existing code style and structure
3. Add unit tests for any new functionality
4. Ensure all tests pass: `pio test`
5. Update documentation as needed
6. Issue your pull request with a clear description

## Development Setup

1. Install PlatformIO Core or IDE
2. Clone your fork: `git clone https://github.com/dapperdivers/chelonian-access.git`
3. Install dependencies: `pio lib install`
4. Run tests: `pio test`

## Code Style Guidelines

- Use meaningful variable and function names
- Follow the existing OOP structure
- Keep functions focused and single-purpose
- Comment complex logic
- Use the established mock objects for testing

## Testing

- Write unit tests for all new functionality
- Ensure tests are isolated and don't depend on hardware
- Use the mock objects in `lib/mocks/`
- Run tests before submitting PRs

## Documentation

- Update README.md if changing user-facing features
- Add inline documentation for complex code
- Update feature documentation in `docs/features/` if implementing enhancements
- Keep examples current and working

## Hardware Safety

⚠️ **CRITICAL**: Always specify that the 3.3V 8MHz Pro Micro is required. The 5V version will damage components!

## Areas Where Help is Needed

1. **Testing new features** - Especially the 14 documented enhancements
2. **PCB design** - For permanent installation
3. **3D printed enclosures** - Weather-resistant cases
4. **Mobile app development** - iOS/Android apps
5. **Security auditing** - Review access control implementation
6. **Power optimization** - Implement sleep modes
7. **Documentation** - Improve guides and examples

## Community

- Be respectful and inclusive
- Help others when you can
- Share your implementations and use cases
- Credit others' contributions

## Questions?

Feel free to open an issue for any questions about contributing!