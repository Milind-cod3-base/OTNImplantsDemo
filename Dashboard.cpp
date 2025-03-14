#include "Dashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent), timeStep(0), currentChartData("Shoulder Angle") {
    setupUI();
    setWindowTitle("OTNImplantsDemo Arm Prosthetic Dashboard");
}

void Dashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Sensor Data Section
    QGroupBox *sensorGroup = new QGroupBox("Sensor Data");
    QVBoxLayout *sensorLayout = new QVBoxLayout(sensorGroup);
    shoulderAngleLabel = new QLabel("Shoulder Angle: 0.0°");
    elbowAngleLabel = new QLabel("Elbow Angle: 0.0°");
    wristAngleLabel = new QLabel("Wrist Angle: 0.0°");
    gripForceLabel = new QLabel("Grip Force: 0.0 N");
    sensorLayout->addWidget(shoulderAngleLabel);
    sensorLayout->addWidget(elbowAngleLabel);
    sensorLayout->addWidget(wristAngleLabel);
    sensorLayout->addWidget(gripForceLabel);
    mainLayout->addWidget(sensorGroup);

    // Actuator Data Section
    QGroupBox *actuatorGroup = new QGroupBox("Actuator Data");
    QVBoxLayout *actuatorLayout = new QVBoxLayout(actuatorGroup);
    shoulderMotorLabel = new QLabel("Shoulder Motor: 0.0°");
    elbowMotorLabel = new QLabel("Elbow Motor: 0.0°");
    wristMotorLabel = new QLabel("Wrist Motor: 0.0°");
    actuatorLayout->addWidget(shoulderMotorLabel);
    actuatorLayout->addWidget(elbowMotorLabel);
    actuatorLayout->addWidget(wristMotorLabel);
    mainLayout->addWidget(actuatorGroup);

    // Chart Section
    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Shoulder Angle History");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);
    chart->axes(Qt::Vertical).first()->setRange(0, 360);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);

    // Control Buttons and Chart Selector (Tip 2)
    QHBoxLayout *controlLayout = new QHBoxLayout();
    startButton = new QPushButton("Start Simulation");
    stopButton = new QPushButton("Stop Simulation");
    chartSelector = new QComboBox();
    chartSelector->addItems({"Shoulder Angle", "Elbow Angle", "Wrist Angle", "Grip Force"});
    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(chartSelector);
    mainLayout->addLayout(controlLayout);

    // Connect buttons and combo box
    connect(startButton, &QPushButton::clicked, this, &Dashboard::onStartClicked);
    connect(stopButton, &QPushButton::clicked, this, &Dashboard::onStopClicked);
    connect(chartSelector, &QComboBox::currentTextChanged, this, &Dashboard::onChartSelectorChanged);

    // Styling (Tip 1)
    setStyleSheet("QGroupBox { font-weight: bold; color: #2c3e50; } "
                  "QLabel { font-size: 14px; color: #34495e; } "
                  "QPushButton { background-color: #3498db; color: white; border: none; padding: 5px; } "
                  "QComboBox { background-color: #ecf0f1; color: #2c3e50; }");
}

void Dashboard::onSensorDataUpdated(SensorData data) {
    shoulderAngleLabel->setText(QString("Shoulder Angle: %1°").arg(data.shoulderAngle, 0, 'f', 1));
    elbowAngleLabel->setText(QString("Elbow Angle: %1°").arg(data.elbowAngle, 0, 'f', 1));
    wristAngleLabel->setText(QString("Wrist Angle: %1°").arg(data.wristAngle, 0, 'f', 1));
    gripForceLabel->setText(QString("Grip Force: %1 N").arg(data.gripForce, 0, 'f', 1));

    if (currentChartData == "Shoulder Angle") {
        updateChart("Shoulder Angle", data.shoulderAngle);
    } else if (currentChartData == "Elbow Angle") {
        updateChart("Elbow Angle", data.elbowAngle);
    } else if (currentChartData == "Wrist Angle") {
        updateChart("Wrist Angle", data.wristAngle);
    } else if (currentChartData == "Grip Force") {
        updateChart("Grip Force", data.gripForce);
    }
}

void Dashboard::onActuatorDataUpdated(ActuatorData data) {
    shoulderMotorLabel->setText(QString("Shoulder Motor: %1°").arg(data.shoulderMotor, 0, 'f', 1));
    elbowMotorLabel->setText(QString("Elbow Motor: %1°").arg(data.elbowMotor, 0, 'f', 1));
    wristMotorLabel->setText(QString("Wrist Motor: %1°").arg(data.wristMotor, 0, 'f', 1));
}

void Dashboard::updateChart(QString dataType, float value) {
    series->append(timeStep++, value);
    if (series->count() > 100) {
        series->remove(0);
    }
    chart->setTitle(dataType + " History");
    chart->axes(Qt::Horizontal).first()->setRange(timeStep - 100, timeStep);
    if (dataType == "Grip Force") {
        chart->axes(Qt::Vertical).first()->setRange(0, 100);
    } else {
        chart->axes(Qt::Vertical).first()->setRange(-30, 360);
    }
}

void Dashboard::onStartClicked() {
    // Start logic handled in main.cpp
}

void Dashboard::onStopClicked() {
    // Stop logic handled in main.cpp
}

void Dashboard::onChartSelectorChanged(const QString &text) {
    currentChartData = text;
    series->clear();
    timeStep = 0;
}
