#include <QApplication>
#include "SerialSimulator.h"
#include "DataProcessor.h"
#include "Dashboard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SerialSimulator simulator;
    DataProcessor processor;
    Dashboard dashboard;

    // Connect signals and slots for OTNImplantsDemo
    QObject::connect(&simulator, &SerialSimulator::dataReceived, &processor, &DataProcessor::processData);
    QObject::connect(&processor, &DataProcessor::sensorDataUpdated, &dashboard, &Dashboard::onSensorDataUpdated);
    QObject::connect(&processor, &DataProcessor::actuatorDataUpdated, &dashboard, &Dashboard::onActuatorDataUpdated);

    // Connect start and stop buttons (Tip 2)
    QObject::connect(dashboard.startButton, &QPushButton::clicked, &simulator, &SerialSimulator::start);
    QObject::connect(dashboard.stopButton, &QPushButton::clicked, &simulator, &SerialSimulator::stop);

    dashboard.show();

    return app.exec();
}
