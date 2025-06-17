# Changelog

All notable changes to Chelonian Access will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Changed

### Added

- GitHub Actions workflow for automated testing
- GitHub Actions workflow for release builds with checksums
- Comprehensive unit test suite for all components

## [1.0.0] - 2025-06-05

### Added
- Initial release of the RFID access control system
- Support for PN532 NFC/RFID reader
- DFPlayer Mini integration for audio feedback
- Relay control for door locks
- Progressive delay system for brute-force protection
- 10-second door unlock timer
- 6 different sound effects for various states
- Power-up confirmation sound
- "Are you still there?" prompt after 10 seconds
- Support for 4-byte and 7-byte RFID UIDs
- Default authorized UIDs configuration
- Comprehensive error handling

### Technical Details
- Built for SparkFun Pro Micro (3.3V, 8MHz)
- Modular architecture with separate controllers
- Unity test framework integration
- PlatformIO build system

## [0.1.0] - 2025-01-01

### Added
- Initial prototype
- Basic RFID reading functionality
- Simple relay control

[Unreleased]: https://github.com/dapperdivers/chelonian-access/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/dapperdivers/chelonian-access/compare/v0.1.0...v1.0.0
[0.1.0]: https://github.com/dapperdivers/chelonian-access/releases/tag/v0.1.0
