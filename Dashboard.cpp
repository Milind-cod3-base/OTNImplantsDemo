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

    // 3. Bottom Group Box: Chart Metrics
    QGroupBox *chartGroup = new QGroupBox("Chart Metrics");
    QHBoxLayout *chartLayout = new QHBoxLayout(chartGroup);

    // Vertical layout for chart selector and its label
    QVBoxLayout *selectorLayout = new QVBoxLayout();
    QLabel *chartSelectorLabel = new QLabel("Motion Metrics Hub");
    chartSelectorLabel->setStyleSheet("color: #ff6200;");
    chartSelector = new QComboBox();
    chartSelector->addItems({"Shoulder Angle", "Elbow Angle", "Wrist Angle", "Grip Force"});
    selectorLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    selectorLayout->addWidget(chartSelectorLabel);
    selectorLayout->addWidget(chartSelector);
    selectorLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    chartLayout->addLayout(selectorLayout);

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

    chartLayout->setStretch(0, 1);
    chartLayout->setStretch(1, 3);
    mainLayout->addWidget(chartGroup);

    connect(chartSelector, &QComboBox::currentTextChanged, this, &Dashboard::onChartSelectorChanged);

    setStyleSheet(R"(
        QWidget { background-color: white; }
        QLabel, QPushButton, QComboBox { color: #ff6200; }
        QGroupBox QLabel { color: black; }
        QGroupBox {
            border: 2px solid #ff6200;
            border-radius: 5px;
            margin-top: 1em;
            color: #ff6200;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 3px;
            color: #ff6200;
        }
        QComboBox {
            background-color: #f0f0f0;
            border: 1px solid #ff6200;
            color: black;
        }
        QComboBox QAbstractItemView {
            background-color: #f0f0f0;
            color: black;
        }
        QPushButton {
            background-color: #ff6200;
            border: none;
            padding: 5px;
            color: white;
        }
    )");

    chart->setBackgroundBrush(QBrush(QColor("#E5E5E5")));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor("#E5E5E5")));
    chart->setPlotAreaBackgroundVisible(true);
    chart->setTitleBrush(QBrush(Qt::black));
    QPen axisPen(Qt::black);
    chart->axes(Qt::Horizontal).first()->setLinePen(axisPen);
    chart->axes(Qt::Vertical).first()->setLinePen(axisPen);
    chart->axes(Qt::Horizontal).first()->setLabelsColor(Qt::black);
    chart->axes(Qt::Vertical).first()->setLabelsColor(Qt::black);
    series->setPen(QPen(QColorConstants::Svg::orange));
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
    // Ensure the X-axis never goes below 0
    qreal xMin = qMax(0, timeStep - 100);  // Start at 0 if timeStep < 100
    chart->axes(Qt::Horizontal).first()->setRange(xMin, timeStep);
    if (dataType == "Grip Force") {
        chart->axes(Qt::Vertical).first()->setRange(0, 100);
    } else {
        chart->axes(Qt::Vertical).first()->setRange(-30, 360);
    }
}

void Dashboard::onStartClicked() {
    // Reset timeStep and clear the chart when the simulation starts
    timeStep = 0;
    series->clear();
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);  // Reset X-axis to start at 0
}

void Dashboard::onStopClicked() {
    // Stop logic handled in main.cpp
}

void Dashboard::onChartSelectorChanged(const QString &text) {
    currentChartData = text;
    series->clear();
    timeStep = 0;
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);  // Reset X-axis to start at 0
}