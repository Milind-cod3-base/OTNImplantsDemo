#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QSplashScreen>  // Added for splash screen
#include <QTimer>         // Added for timing the splash screen
#include "SerialSimulator.h"
#include "DataProcessor.h"
#include "Dashboard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the splash screen using a direct file path
    QPixmap pixmap("logo.png");  // Assumes logo.png is in the same directory as the executable
    if (pixmap.isNull()) {
        // Optional: Handle case where logo.png can't be loaded
        qWarning("Failed to load logo.png - check file path");
    }
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();  // Ensure the splash screen is displayed immediately

    // Initialize your application components
    SerialSimulator simulator;
    DataProcessor processor;
    Dashboard dashboard;

    // Connect signals and slots for OTNImplantsDemo
    QObject::connect(&simulator, &SerialSimulator::dataReceived, &processor, &DataProcessor::processData);
    QObject::connect(&processor, &DataProcessor::sensorDataUpdated, &dashboard, &Dashboard::onSensorDataUpdated);
    QObject::connect(&processor, &DataProcessor::actuatorDataUpdated, &dashboard, &Dashboard::onActuatorDataUpdated);

    // Connect start and stop buttons
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

    // Set the geometry for the dashboard (but don't show it yet)
    dashboard.setGeometry(x, y, width, height);

    // Use a single-shot timer to close splash and show dashboard after 2 seconds
    QTimer::singleShot(2000, [&splash, &dashboard]() {
        splash.close();      // Close the splash screen
        dashboard.show();    // Show the main dashboard
    });

    return app.exec();
}
