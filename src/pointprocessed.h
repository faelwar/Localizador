#ifndef POINTPROCESSED_H
#define POINTPROCESSED_H

#include <QList>

#include "point.h"
#include "rssiprocessed.h"

class PointProcessed
{
private:
    int x;
    int y;

    Direction dir;
public:
    QList<RSSIProcessed> averages;
    PointProcessed();
    PointProcessed(Point p, Direction d);
    PointProcessed(Point *p, Direction d);
    int compareToManhatan(QList<rssi> r);
    double compareToEuclidean(QList<rssi> r);
    int getSignalStrength(AccessPoint ap);
    double getStandardDeviation(AccessPoint ap);
    Direction getDirection() const;
    int getX() const;
    int getY() const;
    void readFromFile(QXmlStreamReader* xmlReader);
    void writeToFile(QXmlStreamWriter* xmlWriter) const;
};


#endif // POINTPROCESSED_H
