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

