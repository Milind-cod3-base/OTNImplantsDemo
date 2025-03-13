
# OTNImplantsDemo

## Overview
`OTNImplantsDemo` is a Qt-based C++ application designed as a demo for OTN Implants, showcasing a dashboard for an arm prosthetic. It simulates sensor and actuator data from an STM32 microcontroller via UART, displaying real-time metrics and charts. The application is built without Qt Designer, using pure C++ for widget and layout creation, and includes a polished UI with custom styles and interactive controls.

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

