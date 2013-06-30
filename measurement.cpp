#include "measurement.h"
#include "rssi.h"

Measurement::Measurement(){

}

Measurement::Measurement(Direction d, QTime t){
    dir = d;
    time = t;
}

Measurement::Measurement(QList<rssi> l, Direction d, QTime t){
    listRSSI = l;
    dir = d;
    time = t;
}

QList<rssi> Measurement::getList(){
    return listRSSI;
}

void Measurement::addData(rssi r){
    listRSSI.append(r);
}

QString Measurement::getTime(){
    return time.toString();
}

int Measurement::getStrength(int index){
    return listRSSI[index].getSignalStrength();
}

QString Measurement::getESSID(int index){
    return listRSSI[index].getESSID();
}
