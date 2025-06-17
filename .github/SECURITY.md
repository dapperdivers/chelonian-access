# Security Policy

## Supported Versions

The following versions of the Chelonian Access project are currently being supported with security updates:

| Version | Supported          |
| ------- | ------------------ |
| main    | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

The Chelonian Access project takes security seriously. We appreciate your efforts to responsibly disclose your findings.

### How to Report

To report a security vulnerability, please:

1. **DO NOT** create a public GitHub issue
2. Email the project maintainers with:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)

### What to Expect

- Acknowledgment of your report within 48 hours
- Regular updates on our progress
- Credit for your discovery (unless you prefer to remain anonymous)

### Security Considerations for This Project

Given the nature of this access control system, please pay special attention to:

1. **Authentication Bypass**
   - RFID cloning vulnerabilities
   - Replay attacks
   - Brute force weaknesses

2. **Physical Security**
   - Relay manipulation
   - Power analysis attacks
   - Hardware tampering

3. **Code Security**
   - Buffer overflows
   - Integer overflows
   - Timing attacks

4. **Configuration Security**
   - Default or weak UIDs
   - Unprotected EEPROM storage
   - Debug information leakage

## Best Practices for Users

1. **Hardware Security**
   - Use the correct 3.3V Pro Micro (5V will damage components)
   - Secure physical access to the control unit
   - Consider adding tamper detection

2. **Software Security**
   - Change default UIDs immediately
   - Enable all security features
   - Keep the firmware updated
   - Review logs regularly (when logging is implemented)

3. **Operational Security**
   - Limit who has authorized cards
   - Implement card revocation procedures
   - Monitor for suspicious access attempts
   - Have an emergency override plan

## Security Features Roadmap

The following security enhancements are planned:

- [ ] Encrypted card storage
- [ ] Rolling codes
- [ ] Multi-factor authentication
- [ ] Secure boot
- [ ] Audit logging
- [ ] Intrusion detection

See the [Feature Roadmap](../docs/features/feature-roadmap.md) for more details.

## Disclaimer

This is an open-source project intended for educational and hobbyist use. While we strive for security, this system should not be used as the sole security measure for high-value assets. Always implement defense in depth.