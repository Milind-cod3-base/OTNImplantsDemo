
/*
 *
 * This file defines the DataProcessor class
 * for parsing UART packets into sensor and actuator data.
 *
 */



#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QByteArray>

// struct for incoming sensordata
struct SensorData {
    float shoulderAngle;
    float elbowAngle;
    float wristAngle;
    float gripForce;
};

// struct to handle outgoing actuator data
struct ActuatorData {
    float shoulderMotor;
    float elbowMotor;
    float wristMotor;
};

class DataProcessor : public QObject {
    Q_OBJECT
public:
    DataProcessor();

signals:
    void sensorDataUpdated(SensorData data);
    void actuatorDataUpdated(ActuatorData data);

public slots:
    void processData(QByteArray packet);
};

#endif // DATAPROCESSOR_H
