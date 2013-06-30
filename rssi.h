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
    rssi(AccessPoint a, int ss);
    QString getMAC();
    QString getESSID();
    int getSignalStrength();
    void setSignalStrength(int ss);
    rssi operator=(rssi r);



};

#endif // RSSI_H
