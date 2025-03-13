
/*
 *
 *  generates or simulates sine-wave-based data for the arm prosthetic.
 */



#include "SerialSimulator.h"

SerialSimulator::SerialSimulator() {
    timer.setInterval(100);  // 100ms = 10Hz update rate for OTNImplantsDemo
    connect(&timer, &QTimer::timeout, this, &SerialSimulator::generateData);
}

void SerialSimulator::start() {
    elapsedTimer.start();
    timer.start();
}

void SerialSimulator::stop() {
    timer.stop();
}

void SerialSimulator::generateData() {
    qint64 t = elapsedTimer.elapsed() / 1000.0;  // Time in seconds

    // Simulate realistic arm movements with sine waves for OTNImplantsDemo
    float shoulderAngle = 180.0 + 90.0 * qSin(2 * M_PI * t / 10.0);  // 90° to 270°
    float elbowAngle = 90.0 + 45.0 * qSin(2 * M_PI * t / 10.0);      // 45° to 135°
    float wristAngle = 0.0 + 30.0 * qSin(2 * M_PI * t / 10.0);       // -30° to 30°
    float gripForce = 50.0 + 50.0 * qSin(2 * M_PI * t / 10.0);       // 0N to 100N

    // Actuator data mirrors sensor data for this demo
    float shoulderMotor = shoulderAngle;
    float elbowMotor = elbowAngle;
    float wristMotor = wristAngle;

    // Convert to uint16_t (scale by 10 for 0.1 precision)
    uint16_t shoulderSend = static_cast<uint16_t>(shoulderAngle * 10.0);
    uint16_t elbowSend = static_cast<uint16_t>(elbowAngle * 10.0);
    uint16_t wristSend = static_cast<uint16_t>(wristAngle * 10.0);
    uint16_t gripSend = static_cast<uint16_t>(gripForce * 10.0);
    uint16_t shoulderMotorSend = static_cast<uint16_t>(shoulderMotor * 10.0);
    uint16_t elbowMotorSend = static_cast<uint16_t>(elbowMotor * 10.0);
    uint16_t wristMotorSend = static_cast<uint16_t>(wristMotor * 10.0);

    // Pack into byte array (little-endian)
    QByteArray data;
    data.append(static_cast<char>(shoulderSend & 0xFF));
    data.append(static_cast<char>(shoulderSend >> 8));
    data.append(static_cast<char>(elbowSend & 0xFF));
    data.append(static_cast<char>(elbowSend >> 8));
    data.append(static_cast<char>(wristSend & 0xFF));
    data.append(static_cast<char>(wristSend >> 8));
    data.append(static_cast<char>(gripSend & 0xFF));
    data.append(static_cast<char>(gripSend >> 8));
    data.append(static_cast<char>(shoulderMotorSend & 0xFF));
    data.append(static_cast<char>(shoulderMotorSend >> 8));
    data.append(static_cast<char>(elbowMotorSend & 0xFF));
    data.append(static_cast<char>(elbowMotorSend >> 8));
    data.append(static_cast<char>(wristMotorSend & 0xFF));
    data.append(static_cast<char>(wristMotorSend >> 8));

    // Calculate checksum
    uint8_t checksum = 0;
    for (char c : data) {
        checksum += static_cast<uint8_t>(c);
    }

    // Build the packet
    QByteArray packet;
    packet.append(static_cast<char>(0xAA));  // Start byte
    packet.append(data);                     // 14 bytes of data
    packet.append(static_cast<char>(checksum));  // Checksum
    packet.append(static_cast<char>(0xBB));  // End byte

    emit dataReceived(packet);
}
