#include "receiver.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>

#include "accesspoint.h"

using namespace std;

Receiver::Receiver(){
    blacklist= QStringList() << "90:F6:52:FB:41:42" << "64:70:02:9F:13:D8" << "00:60:B3:0B:A9:E8" << "00:1A:3F:81:AF:12" << "00:60:B3:0B:9E:82" << "00:1E:58:C2:0B:47" << "00:02:6F:7D:CE:4C" << "02:20:50:C7:2A:8A" << "00:4F:62:03:88:33" << "00:24:01:67:8D:6B" << "02:22:E2:C5:2D:C1" << "D2:05:69:D1:B2:74" << "CC:3A:61:40:77:41" << "3A:08:21:17:EE:88" << "02:23:23:AD:9B:AB" << "02:22:32:AE:64:5F" << "02:23:13:6B:71:6A" << "2A:CF:64:0D:25:6B" << "94:51:03:EC:80:C9" << "00:1A:3F:6E:9D:F2";

}

Receiver::Receiver(string d){
    blacklist= QStringList() << "90:F6:52:FB:41:42" << "64:70:02:9F:13:D8" << "00:60:B3:0B:A9:E8" << "00:1A:3F:81:AF:12" << "00:60:B3:0B:9E:82" << "00:1E:58:C2:0B:47" << "00:02:6F:7D:CE:4C" << "02:20:50:C7:2A:8A" << "00:4F:62:03:88:33" << "00:24:01:67:8D:6B" << "02:22:E2:C5:2D:C1" << "D2:05:69:D1:B2:74" << "CC:3A:61:40:77:41" << "3A:08:21:17:EE:88" << "02:23:23:AD:9B:AB" << "02:22:32:AE:64:5F" << "02:23:13:6B:71:6A" << "2A:CF:64:0D:25:6B" << "94:51:03:EC:80:C9" << "00:1A:3F:6E:9D:F2";
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
    if(!(blacklist.contains(mac))){
        AccessPoint ap(mac, essid);
        rssi r(ap, signal);
        RSSI.append(r);
    }
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
