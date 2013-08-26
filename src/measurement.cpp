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

Direction Measurement::getDirection(){
    return dir;
}

void Measurement::writeToFile(QXmlStreamWriter* xmlWriter){
    xmlWriter->writeStartElement("MEASUREMENT");
    xmlWriter->writeTextElement("Time", time.toString());
    QString direction;
    if(dir == NORTH){
        direction = "NORTH";
    }else if(dir == SOUTH){
        direction = "SOUTH";
    }else if(dir == EAST){
        direction = "EAST";
    }else if(dir == WEST){
        direction = "WEST";
    }
    xmlWriter->writeTextElement("Direction", direction);
    for(int i = 0; i < listRSSI.size(); i++){
        listRSSI[i].writeToFile(xmlWriter);
    }
    xmlWriter->writeEndElement();
}

void Measurement::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <MEASUREMENT>
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "MEASUREMENT")){

        if (xmlReader->isStartElement()){
            if(xmlReader->name() == "Time"){
                xmlReader->readNext();
                time = QTime::fromString(xmlReader->text().toString(),"hh:mm:ss");
                xmlReader->readNext();
            }else if(xmlReader->name() == "Direction"){
                xmlReader->readNext();
                QString aux = xmlReader->text().toString();
                if(aux == "NORTH"){
                    dir = NORTH;
                }else if(aux == "SOUTH"){
                    dir = SOUTH;
                }else if(aux == "EAST"){
                    dir = EAST;
                }else{
                    dir = WEST;
                }
                xmlReader->readNext();
            }else if(xmlReader->name() == "RSSI"){
                rssi r;
                r.readFromFile(xmlReader);
                listRSSI.append(r);
            }
            xmlReader->readNext();
        }else{
            xmlReader->readNext();
        }
    }
    //on the return from this function, all we know is that the token is </MEASUREMENT>
}
