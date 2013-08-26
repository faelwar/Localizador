#include "point.h"
#include <QMessageBox>
#include <iostream>

using namespace std;

Point::Point(int xvalue, int yvalue, int gx, int gy){
    logX = xvalue;
    logY = yvalue;
    guiX = gx;
    guiY = gy;

}

int Point::getNumberOfDataElements(){
    return listRSSI.size();
}

Measurement Point::getMeasurement(int index){
    return listRSSI[index];
}

void Point::addData(Measurement m){

    listRSSI.append(m);

}


int Point::getLogX(){return logX;}
int Point::getLogY(){return logY;}
int Point::getGuiX(){return guiX;}
int Point::getGuiY(){return guiY;}


void Point::writeToFile(QXmlStreamWriter* xmlWriter){
    xmlWriter->writeStartElement("POINT");
        xmlWriter->writeTextElement("LogicalX",QString::number(logX));
        xmlWriter->writeTextElement("LogicalY",QString::number(logY));
        xmlWriter->writeTextElement("GuiX",QString::number(guiX));
        xmlWriter->writeTextElement("GuiY",QString::number(guiY));

        for (int i=0; i < listRSSI.size(); i++){

                Measurement* aux = & listRSSI[i];
                aux->writeToFile(xmlWriter);

        }

    xmlWriter->writeEndElement();
}


void Point::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <POINT>
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "POINT")){

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
            }else if(xmlReader->name() == "MEASUREMENT"){
                Measurement m;
                m.readFromFile(xmlReader);
                listRSSI.append(m);
            }
            xmlReader->readNext();
        }else{
            xmlReader->readNext();
        }
    }
    //on the return from this function, all we know is that the token is </POINT>
}


QList<Measurement> Point::getMeasurement(Direction dir){
    QList<Measurement> result;
    for(int i = 0; i < listRSSI.size(); i++){
        if(listRSSI[i].getDirection() == dir){
            result.append(listRSSI.at(i));
        }
    }
    return result;
}
