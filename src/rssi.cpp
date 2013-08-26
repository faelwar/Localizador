#include "rssi.h"
#include "accesspoint.h"
#include "rssiprocessed.h"

using namespace std;

rssi::rssi(){

}

rssi::rssi(AccessPoint a, int ss){
  ap = a;
  signalStrength = ss;
  //d = dir;
}

QString rssi::getMAC() const{
  return ap.getMAC();
}

QString rssi::getESSID(){
    return ap.getESSID();
}

int rssi::getSignalStrength()const {
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

AccessPoint rssi::getAP(){
    return ap;
}

void rssi::writeToFile(QXmlStreamWriter* xmlWriter){
    xmlWriter->writeStartElement("RSSI");
    xmlWriter->writeTextElement("MACAddress", ap.getMAC());
    xmlWriter->writeTextElement("ESSID", ap.getESSID());
    xmlWriter->writeTextElement("SignalStrength", QString::number(signalStrength));
    xmlWriter->writeEndElement();
}

void rssi::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <RSSI>
    QString m, e;
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "RSSI")){

        if (xmlReader->isStartElement()){
            if(xmlReader->name() == "MACAddress"){
                xmlReader->readNext();
                m = xmlReader->text().toString();
                xmlReader->readNext();
            }else if(xmlReader->name() == "SignalStrength"){
                xmlReader->readNext();
                signalStrength = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "ESSID"){
                xmlReader->readNext();
                e = xmlReader->text().toString();
                xmlReader->readNext();
            }
            xmlReader->readNext();
        }else{
            xmlReader->readNext();
        }
    }
    //on the return from this function, all we know is that the token is </RSSI>
    ap = AccessPoint(m, e);
}

bool rssi::operator==(rssi r){
    return this->getMAC() == r.getMAC();
}
