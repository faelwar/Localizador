#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QList>
#include <QTime>

#include "rssi.h"

enum Direction {NORTH, SOUTH, WEST, EAST};

class Measurement
{
private:
    Direction dir;
    QList<rssi> listRSSI;
    QTime time;
public:
    Measurement();
    Measurement(Direction d, QTime t);
    Measurement(QList<rssi> l, Direction d, QTime t);
    QList<rssi> getList();
    void addData(rssi r);
    int getStrength(int index);
    QString getESSID(int index);
    QString getTime();
};

#endif // MEASUREMENT_H
