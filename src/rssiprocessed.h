#ifndef RSSIPROCESSED_H
#define RSSIPROCESSED_H

#include <accesspoint.h>
#include <measurement.h>

class RSSIProcessed
{
private:
   int averageStrength;
   AccessPoint ap;
   double standardDeviation;


public:
    RSSIProcessed();
    AccessPoint getAP() const;
    int getAverageStrength() const;
    double getStandardDeviation();
    void setAP(AccessPoint ap);
    void setAverageStrength(int s);
    void setStandardDeviation(double s);
    RSSIProcessed(AccessPoint accessPoint, int s, int d);
    void writeToFile(QXmlStreamWriter* xmlWriter);
    void readFromFile(QXmlStreamReader* xmlReader);
};

#endif // RSSIPROCESSED_H
