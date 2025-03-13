/*
 *
 *
 * processes raw packets and emits structured data.
 *
 *
*/



#include "DataProcessor.h"

// Constructor: Initializes the DataProcessor object (currently empty as no initialization is needed)
DataProcessor::DataProcessor() {}

// Processes incoming UART packet data and emits structured sensor and actuator data
void DataProcessor::processData(QByteArray packet) {
    // Validate packet size and start/end bytes to ensure it's a legitimate packet
    if (packet.size() != 17 || packet[0] != static_cast<char>(0xAA) || packet[16] != static_cast<char>(0xBB)) {
        return;  // Invalid packet for OTNImplantsDemo - silently discard
    }

    // Extract the 14 data bytes (excluding start byte, checksum, and end byte)
    QByteArray data = packet.mid(1, 14);  // Starts at index 1, takes 14 bytes

    // Calculate checksum to verify data integrity
    uint8_t checksum = 0;
    for (char c : data) {
        checksum += static_cast<uint8_t>(c);  // Sum all data bytes
    }
    // Compare calculated checksum with the received checksum (at index 15)
    if (static_cast<uint8_t>(packet[15]) != checksum) {
        return;  // Checksum mismatch - discard packet
    }

    // Extract raw 16-bit values from the little-endian byte array
    // Each value is stored in 2 bytes: low byte first, high byte second
    uint16_t shoulderAngleRaw = static_cast<uint16_t>(data[0] & 0xFF) | (static_cast<uint16_t>(data[1] & 0xFF) << 8);
    uint16_t elbowAngleRaw = static_cast<uint16_t>(data[2] & 0xFF) | (static_cast<uint16_t>(data[3] & 0xFF) << 8);
    uint16_t wristAngleRaw = static_cast<uint16_t>(data[4] & 0xFF) | (static_cast<uint16_t>(data[5] & 0xFF) << 8);
    uint16_t gripForceRaw = static_cast<uint16_t>(data[6] & 0xFF) | (static_cast<uint16_t>(data[7] & 0xFF) << 8);
    uint16_t shoulderMotorRaw = static_cast<uint16_t>(data[8] & 0xFF) | (static_cast<uint16_t>(data[9] & 0xFF) << 8);
    uint16_t elbowMotorRaw = static_cast<uint16_t>(data[10] & 0xFF) | (static_cast<uint16_t>(data[11] & 0xFF) << 8);
    uint16_t wristMotorRaw = static_cast<uint16_t>(data[12] & 0xFF) | (static_cast<uint16_t>(data[13] & 0xFF) << 8);

    // Convert raw integer values to floats with 0.1 precision (divide by 10)
    SensorData sensorData;
    sensorData.shoulderAngle = shoulderAngleRaw / 10.0f;  // Convert to degrees
    sensorData.elbowAngle = elbowAngleRaw / 10.0f;        // Convert to degrees
    sensorData.wristAngle = wristAngleRaw / 10.0f;        // Convert to degrees
    sensorData.gripForce = gripForceRaw / 10.0f;          // Convert to Newtons

    ActuatorData actuatorData;
    actuatorData.shoulderMotor = shoulderMotorRaw / 10.0f;  // Convert to degrees
    actuatorData.elbowMotor = elbowMotorRaw / 10.0f;        // Convert to degrees
    actuatorData.wristMotor = wristMotorRaw / 10.0f;        // Convert to degrees

    // Emit the processed data to any connected slots (e.g., Dashboard)
    emit sensorDataUpdated(sensorData);   // Send sensor data to update UI as a signal
    emit actuatorDataUpdated(actuatorData);  // Send actuator data to update UI
}
