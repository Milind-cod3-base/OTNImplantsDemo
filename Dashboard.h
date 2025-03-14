
/*
 *
 * Defines the Dashboard class for the UI
 *
 */


#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "DataProcessor.h"

QT_USE_NAMESPACE

    class Dashboard : public QWidget {
    Q_OBJECT
public:
    Dashboard(QWidget *parent = nullptr);

    // buttons declaration
    QPushButton *startButton;
    QPushButton *stopButton;

private:

    // label declaration
    QLabel *shoulderAngleLabel, *elbowAngleLabel, *wristAngleLabel, *gripForceLabel;
    QLabel *shoulderMotorLabel, *elbowMotorLabel, *wristMotorLabel;


    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    QComboBox *chartSelector;


    int timeStep;
    QString currentChartData;

    void setupUI();
    void updateChart(QString dataType, float value);

public slots:
    void onSensorDataUpdated(SensorData data);
    void onActuatorDataUpdated(ActuatorData data);
    void onStartClicked();
    void onStopClicked();
    void onChartSelectorChanged(const QString &text);
};

#endif // DASHBOARD_H
