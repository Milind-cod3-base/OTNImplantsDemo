#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QSplashScreen>
#include <QTimer>
#include "SerialSimulator.h"
#include "DataProcessor.h"
#include "Dashboard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load and scale the first logo (logo1.png)
    QPixmap pixmap1("logo1.png");  // Assumes logo1.png is in the same directory as the executable
    if (pixmap1.isNull()) {
        qWarning("Failed to load logo1.png - check file path");
    }
    QPixmap scaledPixmap1 = pixmap1.scaled(pixmap1.size() * 1.3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash1(scaledPixmap1);
    splash1.show();
    app.processEvents();  // Ensure the first splash screen is displayed immediately

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

    // Load and scale the second logo (logo2.png) for the second splash screen
    QPixmap pixmap2("logo2.png");  // Assumes logo2.png is in the same directory as the executable
    if (pixmap2.isNull()) {
        qWarning("Failed to load logo2.png - check file path");
    }
    QPixmap scaledPixmap2 = pixmap2.scaled(pixmap2.size() * 1.3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash2(scaledPixmap2);

    // First timer: After 2 seconds, close splash1 and show splash2
    QTimer::singleShot(2000, [&splash1, &splash2]() {
        splash1.close();      // Close the first splash screen
        splash2.show();       // Show the second splash screen
    });

    // Second timer: After 4 seconds total (2 seconds after splash2 shows), close splash2 and show dashboard
    QTimer::singleShot(4000, [&splash2, &dashboard]() {
        splash2.close();      // Close the second splash screen
        dashboard.show();     // Show the main dashboard
    });

    return app.exec();
}
