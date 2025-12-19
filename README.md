# Blackenshovel ESP32-S3 Super Mini

## Overview
This repository contains firmware for the BlackenShovel ESP32-S3 Super Mini platform using PlatformIO (VS Code). Use the guide as a setup reference:
https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/

## Why Platformio?
PlatformIO is more than just an IDE plugin — it’s a complete ecosystem for embedded development. Unlike the Arduino IDE, which is mainly for writing and uploading sketches, PlatformIO handles everything from building and managing projects to library dependency resolution, debugging, and testing. It supports a wide variety of boards and frameworks, allowing developers to reuse code across platforms seamlessly. Its built-in project management, intelligent code completion, and continuous integration support make it ideal for larger, professional-grade projects, where maintaining code quality and scalability is essential.

## Prerequisits
- Python installed
- VS Code with the PlatformIO IDE extension (recommended)
- PlatformIO (installed by the extension)
- ESP32 board and USB cable

## Setup
- Build the project. This will install all labs automatically
- Platformio finds your device port automatically when uploading. In case of any issues, you can check PIO Home -> Devices.
- Setup DEBUG and HTTP_ACTIVE according to your objectives. Setting DEBUG 1 and HTTP_ACTIVE 0 can help you while testing.
    - DEBUG will use the serial monitor to display all logs and also dump the GNSS location (which works only in open fields)
    - HTTP_ACTIVE will make requests to the API if 1. If 0, will use dummy returns.
