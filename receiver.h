#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include <vector>

#include <QList>
#include <QString>

#include "rssi.h"
#include "measurement.h"

using namespace std;

class Receiver{
  private:
    string device;

  public:
    Receiver();
    Receiver(string d);
    QList<rssi> measureRSSI();
    void setDevice(string name);
    string getDevice();
    QList<QString> getAvailableDevices();
};

#endif
