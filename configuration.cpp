#include "configuration.h"

#include "point.h"
#include "accesspoint.h"

#include <QMessageBox>

#include <iostream>

using namespace std;

Configuration::Configuration(int w, int l, int spc, QString file){
    width = w;
    length = l;
    spacing = spc;
    fullFilePath = file;

    gridSpacing = 30; //this is the spacing between points when we draw them


    for(int y=0; y<=length; y+= spacing){
        for(int x=0; x<=width; x+=spacing){
            points.append(new Point(x,y, (x/spacing)*gridSpacing + gridSpacing, (y/spacing)*gridSpacing + gridSpacing));
        }
    }

}

Configuration::Configuration(int w, int l, int spc, QString file, QList<AccessPoint*> list){
    width = w;
    length = l;
    spacing = spc;
    fullFilePath = file;

    apList = list;

    gridSpacing = 30; //this is the spacing between points when we draw them


    for(int y=0; y<=length; y+= spacing){
        for(int x=0; x<=width; x+=spacing){
            points.append(new Point(x,y, (x/spacing)*gridSpacing + gridSpacing, (y/spacing)*gridSpacing + gridSpacing));
        }
    }

    //do something similar for assigning AP gui x and y
    for(int i=0; i<apList.size(); i++){
        apList.at(i)->setguiX(  (int)((double)((double)apList.at(i)->getlogX()/(double)spacing)*gridSpacing + gridSpacing)   );

        apList.at(i)->setguiY(  (int)((double)((double)apList.at(i)->getlogY()/(double)spacing)*gridSpacing + gridSpacing)   );
    }

}

Configuration::Configuration(QString file){
    fullFilePath = file;
    width = 0;
    length = 0;
    spacing = 1;
    gridSpacing = 30;

    //read in values from file
    readFromFile();
}

QVarLengthArray<Point*> * Configuration::getPoints(){
    return &points;
}

QList<AccessPoint*>* Configuration::getAccessPoints(){
    return &apList;
}

int Configuration::getGridSpacing(){
    return gridSpacing;
}

int Configuration::getLogicalSpacing(){
    return spacing;
}

int Configuration::getWidth(){
    return width;
}

int Configuration::getLength(){
    return length;
}

int Configuration::getNumberOfPoints(){
    return points.size();
}

void Configuration::saveToFile(){
    QFile file(fullFilePath);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(2);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("CONFIGURATION");

    xmlWriter.writeTextElement("Width", QString::number(width));
    xmlWriter.writeTextElement("Length", QString::number(length));
    xmlWriter.writeTextElement("Spacing", QString::number(spacing));
    xmlWriter.writeTextElement("GridSpacing", QString::number(gridSpacing));

    for (int j=0; j<apList.size(); j++){
        apList.at(j)->writeToFile(&xmlWriter);
    }

    for (int i=0; i< points.size(); i++){
        points.at(i)->writeToFile(&xmlWriter);
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

void Configuration::readFromFile(){
    QMessageBox* box1 = new QMessageBox();
    QString message1;

    QFile file(fullFilePath);
    QXmlStreamReader xmlReader(&file);
    file.open(QFile::ReadOnly | QFile::Text);
    /////////////////////////////////////////////////////////////////////////


    while(!xmlReader.isEndDocument()){
/*
        message1.append(xmlReader.tokenString());
        message1.append(" , ");
        message1.append(xmlReader.name().toString());
        message1.append(" , ");
        message1.append(xmlReader.text().toString());
        message1.append(" , ");
        message1.append(xmlReader.errorString());
        message1.append("------");

        box1->setText(message1);
        box1->exec();
        xmlReader.readNext();
*/

        if (xmlReader.isStartElement()){
            cout << xmlReader.tokenString().toStdString() <<"="<< xmlReader.name().toString().toStdString() << "\n";
            if(xmlReader.name() == "CONFIGURATION"){
                xmlReader.readNext();
            }else if(xmlReader.name() == "Width"){
                xmlReader.readNext();
                width = xmlReader.text().toString().toInt();
                xmlReader.readNext();
            }else if(xmlReader.name() == "Length"){
                xmlReader.readNext();
                length = xmlReader.text().toString().toInt();
                xmlReader.readNext();
            }else if(xmlReader.name() == "Spacing"){
                xmlReader.readNext();
                spacing = xmlReader.text().toString().toInt();
                xmlReader.readNext();
            }else if(xmlReader.name() == "GridSpacing"){
                xmlReader.readNext();
                gridSpacing = xmlReader.text().toString().toInt();
                xmlReader.readNext();
            }else if(xmlReader.name() == "ACCESSPOINT"){
                //create new access point and call access point readfromfile function
                AccessPoint* ap = new AccessPoint("");
                ap->readFromFile(&xmlReader);
                apList.append(ap);
            }else if(xmlReader.name() == "POINT"){
                //create new point and call point readfromfile function
                Point* p = new Point(0,0,0,0);
                p->readFromFile(&xmlReader);
                points.append(p);
            }
            //cout << xmlReader.tokenString().toStdString() <<"="<< xmlReader.name().toString().toStdString() << "\n";
            xmlReader.readNext();
        }else{
            xmlReader.readNext();
        }
        cout << xmlReader.tokenString().toStdString() <<"="<< xmlReader.name().toString().toStdString() << "\n";


    }
    /////////////////////////////////////////////////////////////////////////
    file.close();
}


Point* Configuration::getPointAt(int x, int y){
    int i = (y/spacing)*((width+spacing)/spacing)+(x/spacing);
    return points.at(i);
}
