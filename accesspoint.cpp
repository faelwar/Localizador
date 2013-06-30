#include "accesspoint.h"

AccessPoint::AccessPoint(){

}

AccessPoint::AccessPoint(QString m, QString e){
    mac = m;
    ESSID = e;
}

AccessPoint::AccessPoint(QString m, int lx, int ly){
    mac = m;
    logX = lx;
    logY = ly;
}

AccessPoint::AccessPoint(const AccessPoint& a){
    this->mac = a.mac;
    this->logX = a.logX;
    this->logY = a.logY;
    this->ESSID = a.ESSID;
}

AccessPoint AccessPoint::operator=(AccessPoint a){
    this->mac = a.mac;
    this->logX = a.logX;
    this->logY = a.logY;
    this->ESSID = a.ESSID;
    return *this;
}

QString AccessPoint::getMAC(){return mac;}
QString AccessPoint::getESSID(){return ESSID;}
int AccessPoint::getlogX(){return logX;}
int AccessPoint::getlogY(){return logY;}
int AccessPoint::getguiX(){return guiX;}
int AccessPoint::getguiY(){return guiY;}
void AccessPoint::setguiX(int x){guiX = x;}
void AccessPoint::setguiY(int y){guiY = y;}

void AccessPoint::writeToFile(QXmlStreamWriter* xmlWriter){
    xmlWriter->writeStartElement("ACCESSPOINT");
        xmlWriter->writeTextElement("MACAddress", mac);
        xmlWriter->writeTextElement("LogicalX",QString::number(logX));
        xmlWriter->writeTextElement("LogicalY",QString::number(logY));
        xmlWriter->writeTextElement("GuiX",QString::number(guiX));
        xmlWriter->writeTextElement("GuiY",QString::number(guiY));
    xmlWriter->writeEndElement();
}

void AccessPoint::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <ACCESSPOINT>
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "ACCESSPOINT")){

        if (xmlReader->isStartElement()){
            if(xmlReader->name() == "LogicalX"){
                xmlReader->readNext();
                logX = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "LogicalY"){
                xmlReader->readNext();
                logY = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "GuiX"){
                xmlReader->readNext();
                guiX = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "GuiY"){
                xmlReader->readNext();
                guiY = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "MACAddress"){
                xmlReader->readNext();
                mac = xmlReader->text().toString();
                xmlReader->readNext();
            }
            xmlReader->readNext();
        }else{
            xmlReader->readNext();
        }
    }
    //on the return from this function, all we know is that the token is </ACCESSPOINT>
}
