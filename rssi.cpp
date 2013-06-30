#include "rssi.h"
#include "accesspoint.h"

using namespace std;

rssi::rssi(AccessPoint a, int ss){
  ap = a;
  signalStrength = ss;
  //d = dir;
}

QString rssi::getMAC(){
  return ap.getMAC();
}

QString rssi::getESSID(){
    return ap.getESSID();
}

int rssi::getSignalStrength(){
  return signalStrength;
}

void rssi::setSignalStrength(int ss){
  signalStrength = ss;
}


rssi rssi::operator=(rssi r){
  signalStrength = r.signalStrength;
  ap = r.ap;
  return *this;
}
