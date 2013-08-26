#ifndef RSSI_H
#define RSSI_H

#include <string>

#include "accesspoint.h"

using namespace std;



class rssi{
  private:
    AccessPoint ap;
    int signalStrength;
  public:
    rssi();
    rssi(AccessPoint a, int ss);
    QString getMAC() const;
    QString getESSID();
    AccessPoint getAP();
    int getSignalStrength() const;
    void setSignalStrength(int ss);
    rssi operator=(rssi r);
    void writeToFile(QXmlStreamWriter* xmlWriter);
    void readFromFile(QXmlStreamReader* xmlReader);
    bool operator==(rssi r);


};

#endif // RSSI_H
