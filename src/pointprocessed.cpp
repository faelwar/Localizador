#include "pointprocessed.h"


#include <cmath>

#include <QList>
#include <QHash>

#include "point.h"
#include "rssiprocessed.h"
#include "direction.h"
#include "blacklist.h"
#include "receiver.h"

PointProcessed::PointProcessed()
{
}

PointProcessed::PointProcessed(Point p, Direction d){
    x = p.getGuiX();
    y = p.getGuiY();
    QList<Measurement> l = p.getMeasurement(d);
    QHash<QString,QList<int> > list;
    QHash<QString,QString> essid;
    for(int i = 0; i < l.size(); i++){
        for(int j =0; j < l[i].getList().size(); j++){
            if(list.contains(l[i].getList()[j].getMAC())){
                list[l[i].getList()[j].getMAC()].append(l[i].getList()[j].getSignalStrength());
            }else{
                list.insert(l[i].getList()[j].getMAC(),QList<int>());
                essid.insert(l[i].getList()[j].getMAC(), l[i].getList()[j].getESSID());
                list[l[i].getList()[j].getMAC()].append(l[i].getList()[j].getSignalStrength());
            }
        }
    }
    QList<QString> mac = list.keys();
    Receiver r;
    for(int i = 0; i < mac.size(); i++){
        if(!(r.blacklist.contains(mac.at(i)))){
            double average = 0;
            for(int j = 0; j < list[mac[i]].size(); j++){
                average += list[mac[i]].at(j);
            }
            average /= list[mac[i]].size();
            double deviation = 0;
            AccessPoint ap(mac[i],essid[mac[i]]);
            RSSIProcessed r(ap, average,deviation);
            this->averages.append(r);
        }
    }
    this->dir = d;
}

void PointProcessed::writeToFile(QXmlStreamWriter* xmlWriter) const{
    xmlWriter->writeStartElement("POINT");
        xmlWriter->writeTextElement("X",QString::number(this->getX()));
        xmlWriter->writeTextElement("Y",QString::number(this->getY()));
        Direction d = this->getDirection();
        QString dir;
        if(d == NORTH){
            dir = "NORTH";
        }else if(d == SOUTH){
            dir = "SOUTH";
        }else if(d == WEST){
            dir = "WEST";
        }else{
            dir = "EAST";
        }
        xmlWriter->writeTextElement("DIRECTION",dir);
        for (int i=0; i < this->averages.size(); i++){
                RSSIProcessed aux = averages[i];
                aux.writeToFile(xmlWriter);
        }

    xmlWriter->writeEndElement();
}

void PointProcessed::readFromFile(QXmlStreamReader* xmlReader){
    //all we know upon entering the function is that the current token is <POINT>
    while(!(xmlReader->isEndElement()) || !(xmlReader->name() == "POINT")){

        if (xmlReader->isStartElement()){
            if(xmlReader->name() == "X"){
                xmlReader->readNext();
                x = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "Y"){
                xmlReader->readNext();
                y = xmlReader->text().toString().toInt();
                xmlReader->readNext();
            }else if(xmlReader->name() == "DIRECTION"){
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
                RSSIProcessed m;
                m.readFromFile(xmlReader);
                if(m.getAP().getESSID().size() < 50){
                    averages.append(m);
                }
            }
            xmlReader->readNext();
        }else{
            xmlReader->readNext();
        }
    }
    //on the return from this function, all we know is that the token is </POINT>
}


PointProcessed::PointProcessed(Point* p, Direction d){
    x = p->getGuiX();
    y = p->getGuiY();
    QList<Measurement> l = p->getMeasurement(d);
    QHash<QString,QList<int> > list;
    for(int i = 0; i < l.size(); i++){
        for(int j =0; j < l[i].getList().size(); j++){
            if(list.contains(l[i].getList()[j].getMAC())){
                list[l[i].getList()[j].getMAC()].append(l[i].getList()[j].getSignalStrength());
            }else{
                list.insert(l[i].getList()[j].getMAC(),QList<int>());
                list[l[i].getList()[j].getMAC()].append(l[i].getList()[j].getSignalStrength());
            }
        }
    }
    QList<QString> mac = list.keys();
    for(int i = 0; i < mac.size(); i++){
        double average = 0;
        for(int j = 0; j < list[mac[i]].size(); j++){
            average += list[mac[i]].at(j);
        }
        average /= list[mac[i]].size();
        double deviation = 0;
        for(int j = 0; j < list[mac[i]].size(); j++){
            deviation = pow(list[mac[i]].at(j)-average,2);
        }
        deviation /= list[mac[i]].size();
        deviation = sqrt(deviation);
        AccessPoint ap(mac[i],"");
        RSSIProcessed r(ap, average,deviation);
        this->averages.append(r);
    }
    this->dir = d;
}


int PointProcessed::compareToManhatan(QList<rssi> r){
    int sum = 0;
    int m = 0;

    for(int i = 0; i <averages.size(); i++){
        rssi aux(averages.at(i).getAP(), averages.at(i).getAverageStrength());
        int index = r.indexOf(aux);
        if(index != -1){
            int auxSS = getSignalStrength(averages[i].getAP());
            int auxSD = getStandardDeviation(averages[i].getAP());
            if(auxSS != 0){
                m++;
                int dif = auxSS - r[index].getSignalStrength();
                if(dif < 0){
                    dif *= -1;
                }
                dif = dif - auxSD;
                if(dif > 0){
                    sum += dif;
                }
            }
        }else{
            int auxSS = getSignalStrength(averages[i].getAP());
            sum += auxSS*-1;
        }
    }
    return sum;
}


int PointProcessed::getX()const{
    return x;
}

int PointProcessed::getY() const{
    return y;
}

double PointProcessed::compareToEuclidean(QList<rssi> r){
    int sum = 0;
    int matches = 0;
    for(int i = 0; i < r.size(); i++){
        int auxSS = getSignalStrength(r[i].getAP());
        int auxSD = getStandardDeviation(r[i].getAP());
        if(auxSS != 0){
            matches++;
            int dif = auxSS - r[i].getSignalStrength();
            if(dif < 0){
                dif *= -1;
            }
            dif = dif - auxSD;
            if(dif > 0){
                dif = pow(dif, 2);
                sum += dif;
            }
        }
    }
    if(matches == 0){
        return 99999999;
    }
    return sqrt(sum/matches);
}

int PointProcessed::getSignalStrength(AccessPoint ap){
    for(int i = 0; i < averages.size(); i++){
        if(averages[i].getAP().getMAC() == ap.getMAC()){
            return averages[i].getAverageStrength();
        }
    }
    return 0;
}

double PointProcessed::getStandardDeviation(AccessPoint ap){
    for(int i = 0; i < averages.size(); i++){
        if(averages[i].getAP().getMAC() == ap.getMAC()){
            return averages[i].getStandardDeviation();
        }
    }
    return 0;
}

Direction PointProcessed::getDirection() const{
    return dir;
}
