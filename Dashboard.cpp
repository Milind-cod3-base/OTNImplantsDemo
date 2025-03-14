#include "Dashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent), timeStep(0), currentChartData("Shoulder Angle") {
    setupUI();
    setWindowTitle("OTNImplantsDemo Arm Prosthetic Dashboard");
}

void Dashboard::setupUI() {
    // Main vertical layout for the entire UI
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 1. Top Horizontal Layout: Start and Stop Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("Start Simulation");
    stopButton = new QPushButton("Stop Simulation");
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    mainLayout->addLayout(buttonLayout);

    // 2. Middle Horizontal Layout: Sensor and Actuator Group Boxes
    QHBoxLayout *groupLayout = new QHBoxLayout();

    // Sensor Data Group Box
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
    groupLayout->addWidget(sensorGroup);

    // Actuator Data Group Box
    QGroupBox *actuatorGroup = new QGroupBox("Actuator Data");
    QVBoxLayout *actuatorLayout = new QVBoxLayout(actuatorGroup);
    shoulderMotorLabel = new QLabel("Shoulder Motor: 0.0°");
    elbowMotorLabel = new QLabel("Elbow Motor: 0.0°");
    wristMotorLabel = new QLabel("Wrist Motor: 0.0°");
    actuatorLayout->addWidget(shoulderMotorLabel);
    actuatorLayout->addWidget(elbowMotorLabel);
    actuatorLayout->addWidget(wristMotorLabel);
    groupLayout->addWidget(actuatorGroup);

    mainLayout->addLayout(groupLayout);

    // 3. Bottom Horizontal Layout: Chart Selector and Chart
    QHBoxLayout *chartLayout = new QHBoxLayout();
    chartSelector = new QComboBox();
    chartSelector->addItems({"Shoulder Angle", "Elbow Angle", "Wrist Angle", "Grip Force"});
    chartLayout->addWidget(chartSelector);

    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Shoulder Angle History");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);
    chart->axes(Qt::Vertical).first()->setRange(0, 360);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(chartView);

    // Adjust stretch factors: Chart takes more space than the selector
    chartLayout->setStretch(0, 1); // Chart selector
    chartLayout->setStretch(1, 3); // Chart view
    mainLayout->addLayout(chartLayout);

    // Connect the chart selector for dynamic updates
    connect(chartSelector, &QComboBox::currentTextChanged, this, &Dashboard::onChartSelectorChanged);

    // Apply Dark Mode Stylesheet
    setStyleSheet(R"(
        /* Main background */
        QWidget {
            background-color: black;
        }

        /* Text visibility on black background */
        QLabel, QPushButton, QComboBox {
            color: white;
        }

        /* Thick group box borders */
        QGroupBox {
            border: 2px solid white;
            border-radius: 5px;
            margin-top: 1em;
            color: white;
        }

        /* Group box title styling */
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 3px;
            color: white;
        }

        /* Combo box styling for dark theme */
        QComboBox {
            background-color: #2c3e50;
            border: 1px solid white;
        }
        QComboBox QAbstractItemView {
            background-color: #2c3e50;
            color: white;
        }

        /* Button styling */
        QPushButton {
            background-color: #3498db;
            border: none;
            padding: 5px;
        }
    )");

    // Configure Chart for Dark Mode
    chart->setBackgroundBrush(QBrush(Qt::black));
    chart->setPlotAreaBackgroundBrush(QBrush(Qt::black));
    chart->setPlotAreaBackgroundVisible(true);
    chart->setTitleBrush(QBrush(Qt::white));
    QPen axisPen(Qt::white);
    chart->axes(Qt::Horizontal).first()->setLinePen(axisPen);
    chart->axes(Qt::Vertical).first()->setLinePen(axisPen);
    chart->axes(Qt::Horizontal).first()->setLabelsColor(Qt::white);
    chart->axes(Qt::Vertical).first()->setLabelsColor(Qt::white);
    series->setPen(QPen(Qt::white));
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
