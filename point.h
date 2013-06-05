#ifndef POINT_H
#define POINT_H

#include <QList>
#include <QString>
#include <QVarLengthArray>
#include <QTime>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Point
{
public:
    Point(int xvalue,int yvalue, int gx, int gy);

    void addData(QString mac, QTime t, int s);
    QString getMAC(int i);
    QTime getTime(int i);
    int getStrength(int i);
    int getLogX();
    int getLogY();
    int getGuiX();
    int getGuiY();
    void writeToFile(QXmlStreamWriter* xmlWriter);
    void readFromFile(QXmlStreamReader* xmlReader);
    int getNumberOfDataElements();

private:
    int logX;
    int logY;
    int guiX;
    int guiY;
    QList<QString> macAddress;
    QList<QTime> time;
    QList<int> strength;
};

#endif // POINT_H
