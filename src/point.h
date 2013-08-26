#ifndef POINT_H
#define POINT_H

#include <QList>
#include <QString>
#include <QVarLengthArray>
#include <QTime>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "measurement.h"

#include "rssi.h"

class Point
{
public:
    Point(int xvalue,int yvalue, int gx, int gy);
    void addData(Measurement m);
    int getLogX();
    int getLogY();
    int getGuiX();
    int getGuiY();
    void writeToFile(QXmlStreamWriter* xmlWriter);
    void readFromFile(QXmlStreamReader* xmlReader);
    int getNumberOfDataElements();
    Measurement getMeasurement(int index);
    QList<Measurement> getMeasurement(Direction dir);

private:
    int logX;
    int logY;
    int guiX;
    int guiY;
    QList<Measurement> listRSSI;

};

#endif // POINT_H
