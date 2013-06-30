#include "receiver.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>

#include "accesspoint.h"

using namespace std;

Receiver::Receiver(){

}

Receiver::Receiver(string d){
    device = d;
}

QList<rssi> Receiver::measureRSSI(){
  FILE *fp;
  char BUF[1024];
  char cmd[100] = "iwlist ";
  string result = "";
  strcat(cmd, device.c_str());
  strcat(cmd, " scanning");
  fp = popen(cmd, "r");
  if (fp){
    while (!feof(fp))
    {
      if (fgets(BUF, 1024, fp) != NULL)
      {
        result += BUF;
      }
    }
    pclose(fp);
  }
  QList<rssi> RSSI;
  int aux = result.find("Address", 0);
  while(aux !=-1){
    QString mac = QString((result.substr(aux + 9, 17)).c_str());
    aux = result.find("Signal", aux);
    int signal = atoi(result.substr(aux + 13, 4).c_str());
    aux = result.find("ESSID", aux);
    QString essid = QString((result.substr(aux + 7, (result.find("\"", aux + 8)) - (aux+7))).c_str());
    aux = result.find("Address", aux);

    AccessPoint ap(mac, essid);
    rssi r(ap, signal);
    RSSI.append(r);
  }
  //cout << result << endl;
  return RSSI;
}

void Receiver::setDevice(string name){
  device = name;
}

string Receiver::getDevice(){
  return device;
}

QList<QString> Receiver::getAvailableDevices(){
  FILE *fp;
  char BUF[1024];
  char cmd[100] = "ip link show";
  string result = "";
  fp = popen(cmd, "r");
  if (fp){
    while (!feof(fp))
    {
      if (fgets(BUF, 1024, fp) != NULL)
      {
    result += BUF;
      }
    }
    pclose(fp);
  }
  QList<QString> devices;
  int aux = result.find("wlan", 0);
  while(aux !=-1){
    QString d = QString(result.substr(aux, 5).c_str());
    aux = result.find("wlan", aux + 1);
    devices.append(d);
  }
  return devices;
}
