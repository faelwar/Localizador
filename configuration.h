#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <QVarLengthArray>
#include <QList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

class Point;
class AccessPoint;

class Configuration
{
public:
    Configuration(int w, int l, int spc, QString file);
    Configuration(QString file, QList<AccessPoint*> list);
    Configuration(int w, int l, int spc, QString file, QList<AccessPoint*> list);
    Configuration(QString file);

    int getGridSpacing();
    int getLogicalSpacing();
    int getWidth();
    int getLength();
    void saveToFile();
    QVarLengthArray<Point*> * getPoints();
    QList<AccessPoint*> * getAccessPoints();
    int getNumberOfPoints();
    Point* getPointAt(int x, int y);

private:
    int width;
    int length;
    int spacing;
    int gridSpacing;
    QString fullFilePath;
    QVarLengthArray <Point*> points;
    QList<AccessPoint*> apList;
    void readFromFile();
};

#endif // CONFIGURATION_H
