#include "rssiprocessed.h"

RSSIProcessed::RSSIProcessed()
{

}


AccessPoint RSSIProcessed::getAP() const{
    return ap;
}

int RSSIProcessed::getAverageStrength() const {
    return averageStrength;
}

double RSSIProcessed::getStandardDeviation(){
    return standardDeviation;
}


void RSSIProcessed::setAP(AccessPoint ap){
    this->ap = ap;
}

void RSSIProcessed::setAverageStrength(int s){
    averageStrength = s;
}

void RSSIProcessed::setStandardDeviation(double s){
    standardDeviation = s;
}


RSSIProcessed::RSSIProcessed(AccessPoint accessPoint, int s, int d){
    setAP(accessPoint);
    setAverageStrength(s);
    setStandardDeviation(d);
}

void RSSIProcessed::writeToFile(QXmlStreamWriter* xmlWriter){
    xmlWriter->writeStartElement("RSSI");
    xmlWriter->writeTextElement("MACAddress", ap.getMAC());
    xmlWriter->writeTextElement("ESSID", ap.getESSID());
    xmlWriter->writeTextElement("AverageSignalStrength", QString::number(averageStrength));
    xmlWriter->writeTextElement("StandardDeviation", QString::number(standardDeviation));
    xmlWriter->writeEndElement();
}
void RSSIProcessed::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <RSSI>
    QString m, e;
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "RSSI")){

        if (xmlReader->isStartElement()){
            if(xmlReader->name() == "MACAddress"){
                xmlReader->readNext();
                m = xmlReader->text().toString();
                xmlReader->readNext();
            }else if(xmlReader->name() == "AverageSignalStrength"){
                xmlReader->readNext();
                averageStrength = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "StandardDeviation"){
                xmlReader->readNext();
                standardDeviation = xmlReader->text().toString().toInt();
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
