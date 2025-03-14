#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
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

    // Get the primary screen's geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Calculate 80% of screen width and height
    int width = screenGeometry.width() * 0.8;
    int height = screenGeometry.height() * 0.8;

    // Center the window
    int x = (screenGeometry.width() - width) / 2;
    int y = (screenGeometry.height() - height) / 2;

    // Set the geometry and show the dashboard
    dashboard.setGeometry(x, y, width, height);
    dashboard.show();

    return app.exec();
}
