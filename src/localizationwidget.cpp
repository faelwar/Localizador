#include <QString>
#include <math.h>
#include <QFuture>
#include <iostream>
#include <QHash>

#include "localizationwidget.h"
#include "ui_localizationwidget.h"

#include "mainwindow.h"
#include "configuration.h"
#include "localizationguiwidget.h"
#include "point.h"
#include "distance.h"



using namespace std;

LocalizationWidget::LocalizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalizationWidget)
{
    ui->setupUi(this);

}

LocalizationWidget::~LocalizationWidget()
{

    delete ui;
}

LocalizationWidget::LocalizationWidget(MainWindow* mw) :
    ui(new Ui::LocalizationWidget)
{
    ui->setupUi(this);
    mainwindow = mw;

    localizationGui = new LocalizationGuiWidget(this);

    ui->scrollArea->setWidget(localizationGui);

    capturing = false;

    QObject::connect(ui->measure, SIGNAL(clicked()), this, SLOT(measure()));
    QObject::connect(ui->realTime, SIGNAL(clicked()),this, SLOT(realTime()));
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(measure()));
    realTimeOn = false;



}

void LocalizationWidget::setConfiguration(Configuration* c, int aux){
    config = c;

    localizationGui->setConfiguration(c);
    if(aux == 1){
        for(int i = 0; i < config->getPoints()->size();i++){
            cout << "Processing point "<< i + 1 << " of " << config->getPoints()->size() << endl;
            PointProcessed north(config->getPoints()->at(i),NORTH);
            PointProcessed south(config->getPoints()->at(i),SOUTH);
            PointProcessed west(config->getPoints()->at(i),WEST);
            PointProcessed east(config->getPoints()->at(i),EAST);
            this->points.append(north);
            this->points.append(south);
            this->points.append(west);
            this->points.append(east);
        }
        QFile file(config->getFilePath() + ".pos");
        file.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.setAutoFormattingIndent(2);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("CONFIGURATION");

        for (int j=0; j<config->getAccessPoints()->size(); j++){
            config->getAccessPoints()->at(j)->writeToFile(&xmlWriter);
        }

        for (int i=0; i< points.size(); i++){
            points.at(i).writeToFile(&xmlWriter);
        }

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }else if(aux == 0){
        QFile file(config->getFilePath());
        QXmlStreamReader xmlReader(&file);
        file.open(QFile::ReadOnly | QFile::Text);
        /////////////////////////////////////////////////////////////////////////


        while(!xmlReader.isEndDocument()){

            if (xmlReader.isStartElement()){
                cout << xmlReader.tokenString().toStdString() <<"="<< xmlReader.name().toString().toStdString() << "\n";
                if(xmlReader.name() == "CONFIGURATION"){
                    xmlReader.readNext();
                }else if(xmlReader.name() == "ACCESSPOINT"){
                    //create new access point and call access point readfromfile function
                    AccessPoint* ap = new AccessPoint("","");
                    ap->readFromFile(&xmlReader);
                    if(ap->getESSID().size() < 50){
                        config->getAccessPoints()->append(ap);
                    }
                }else if(xmlReader.name() == "POINT"){
                    //create new point and call point readfromfile function
                    PointProcessed p;
                    p.readFromFile(&xmlReader);
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
    Receiver r;
    QList<QString> devs = r.getAvailableDevices();
    for(int i = 0; i < devs.size(); i++){
        ui->devices->addItem(devs[i]);
    }
}

void LocalizationWidget::calculatePosition(){
    cout << "Measuring..." << endl;
    string device = ui->devices->currentText().toStdString();
    Receiver r(device);
    QList<rssi> now;
    QHash<QString, QList<int> > list;
    for(int i = 0; i < ui->spinBox->value(); i++){
        do{
            now = r.measureRSSI();
        }while(now.size() == 0);
        for(int j = 0; j < now.size(); j++){
            if(list.contains(now.at(j).getMAC())){
                list[now.at(j).getMAC()].append(now.at(j).getSignalStrength());
            }else{
                list.insert(now.at(j).getMAC(),QList<int>());
                list[now.at(j).getMAC()].append(now.at(j).getSignalStrength());
            }
        }
    }
    now.clear();
    QList<QString> macs = list.keys();
    for(int i = 0; i < macs.size(); i++){
        QList<int> ss =  list[macs.at(i)];
        int sum = 0;
        for(int j = 0; j < ss.size(); j++){
            sum += ss.at(j);
        }
        now.append(rssi(AccessPoint(macs.at(i),""),sum/ss.size()));
    }
    for(int i = 0; i< now.size(); i++){
        cout << now.at(i).getMAC().toStdString() << "="<< now.at(i).getSignalStrength() << endl;
    }
    QList<Distance> manhattan;
    for(int i = 0; i < points.size();i++){
        int d = points[i].compareToManhatan(now);
        manhattan.append(Distance(&points[i],d));
    }
    qSort(manhattan);
    PointProcessed* selectedPoint = manhattan[0].getPoint();
    this->localizationGui->updateUserPositionManhantan(selectedPoint->getX(),selectedPoint->getY(), selectedPoint->getDirection());

    ui->results->setItem(0,0, new QTableWidgetItem(QString::number(selectedPoint->getX())));
    ui->results->setItem(0,1, new QTableWidgetItem(QString::number(selectedPoint->getY())));
    ui->results->setItem(0,2, new QTableWidgetItem(QString::fromStdString(directionToString(selectedPoint->getDirection()))));
    QList<Distance> euclidean;
    for(int i = 0; i < points.size();i++){
        double d = points[i].compareToEuclidean(now);
        euclidean.append(Distance(&points[i], d));
    }
    qSort(euclidean);
    selectedPoint = euclidean[0].getPoint();
    this->localizationGui->updateUserPositionEuclidean(selectedPoint->getX(),selectedPoint->getY(), selectedPoint->getDirection());
    ui->results->setItem(1,0, new QTableWidgetItem(QString::number(selectedPoint->getX())));
    ui->results->setItem(1,1, new QTableWidgetItem(QString::number(selectedPoint->getY())));
    ui->results->setItem(1,2, new QTableWidgetItem(QString::fromStdString(directionToString(selectedPoint->getDirection()))));

    int sumEX = 0, sumEY = 0;
    int sumMX = 0, sumMY = 0;
    for(int i = 0; i < ui->MValue->value(); i++){
        sumEX += euclidean[i].getPoint()->getX();
        sumEY += euclidean[i].getPoint()->getY();
        sumMX += manhattan[i].getPoint()->getX();
        sumMY += manhattan[i].getPoint()->getY();
    }
    int closestEX = sumEX/ui->MValue->value();
    int closestEY = sumEY/ui->MValue->value();
    int closestMX = sumMX/ui->MValue->value();
    int closestMY = sumMY/ui->MValue->value();

    this->localizationGui->updateUserMClosestManhattanPosition(closestMX, closestMY);
    ui->results->setItem(2,0, new QTableWidgetItem(QString::number(closestMX)));
    ui->results->setItem(2,1, new QTableWidgetItem(QString::number(closestMY)));
    ui->results->setItem(2,2, new QTableWidgetItem("No Direction"));

    this->localizationGui->updateUserMClosestEuclideanPosition(closestEX, closestEY);
    ui->results->setItem(3,0, new QTableWidgetItem(QString::number(closestEX)));
    ui->results->setItem(3,1, new QTableWidgetItem(QString::number(closestEY)));
    ui->results->setItem(3,2, new QTableWidgetItem("No Direction"));

}

void LocalizationWidget::measure(){
    calculatePosition();
}

void LocalizationWidget::realTime(){
    if(!realTimeOn){
        ui->realTime->setText("Localization ON");
        timer->start(1000);
        realTimeOn = true;
    }else{
        realTimeOn = false;
        ui->realTime->setText("Localization OFF");
        timer->stop();
    }
}

string LocalizationWidget::directionToString(Direction d){
    if(d == NORTH){
        return "NORTH";
    }
    if(d == SOUTH){
        return "SOUTH";
    }
    if(d == EAST){
        return "EAST";
    }
    if(d == WEST){
        return "WEST";
    }
    return "WRONG";
}

