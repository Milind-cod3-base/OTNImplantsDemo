#ifndef SERIALSIMULATOR_H
#define SERIALSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QByteArray>
#include <QtMath>

/*
 *
 * SerialSimulator class is used to generate dummy UART packets
 *
 */

class SerialSimulator : public QObject {
    Q_OBJECT
public:
    SerialSimulator();
    void start(); // in public to be accessed outside the class
    void stop();

signals:
    void dataReceived(QByteArray data); // signal

private:
    QTimer timer;
    QElapsedTimer elapsedTimer;

    void generateData();
};

#endif // SERIALSIMULATOR_H
