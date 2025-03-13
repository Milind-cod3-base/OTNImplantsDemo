
# OTNImplantsDemo

## Overview
`OTNImplantsDemo` is a Qt-based C++ application designed as a demo for OTN Implants, showcasing a dashboard for an arm prosthetic. It simulates sensor and actuator data from an STM32 microcontroller via UART, displaying real-time metrics and charts. 


### Features
- Displays sensor data (shoulder angle, elbow angle, wrist angle, grip force) and actuator data (motor positions).
- Real-time chart visualization with selectable data types (shoulder angle, elbow angle, wrist angle, grip force).
- Start/Stop simulation buttons for controlling data generation.
- Custom-styled UI using Qt stylesheets.
- Built with C++17 and Qt 6.x, targeting Windows (MinGW).

## Project Structure
	OTNImplantsDemo/
	├── main.cpp
	├── SerialSimulator.h
	├── SerialSimulator.cpp
	├── DataProcessor.h
	├── DataProcessor.cpp
	├── Dashboard.h
	├── Dashboard.cpp
	├── OTNImplantsDemo.pro
	├── README.md


## File Descriptions

### `OTNImplantsDemo.pro`
- **Purpose**: Qt project configuration file.
- **Details**: Specifies Qt modules (`core`, `gui`, `charts`, `widgets`), C++17 standard, source/header files, and build settings. Outputs the executable to a `test` subdirectory.

### `SerialSimulator.h`
- **Purpose**: Header file for the `SerialSimulator` class.
- **Details**: Declares a class that simulates UART data packets from an STM32, including methods to start/stop simulation and emit data signals.

### `SerialSimulator.cpp`
- **Purpose**: Implementation of the `SerialSimulator` class.
- **Details**: Generates dummy sensor/actuator data using sine waves, packs it into UART-like packets with checksums, and emits them every 100ms when started.
