#include <QString>
#include <math.h>
#include <QFuture>
#include <QtConcurrentRun>
#include <iostream>

#include "localizationwidget.h"
#include "ui_localizationwidget.h"

#include "mainwindow.h"
#include "configuration.h"
#include "localizationguiwidget.h"
#include "point.h"



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



}

void LocalizationWidget::setConfiguration(Configuration* c){
    config = c;

    int gridspace = config->getGridSpacing();
    int width = config->getWidth();
    int length = config->getLength();
    int spacing = config->getLogicalSpacing();

    localizationGui->resize(2*gridspace + gridspace*(width/spacing), 2*gridspace + gridspace*(length/spacing));
    localizationGui->setConfiguration(c);
}

void LocalizationWidget::calculatePosition(){
    QVarLengthArray<Point*>* points = config->getPoints();
    int numAP = config->getAccessPoints()->count();
    //while(true){
        SignalStrength* ap = new SignalStrength[numAP];
        //Receiver r;
        if(r.init(numAP)==OK){
            ap = r.Receive();
            //r.shutdown();
        }
        int min = 0;
        double* d = new double[points->count()];
        for(int i = 0; i < points->count(); i++){
            int sum = 0;
            if(points->at(i)->getNumberOfDataElements() > 0){
                for(int j = 0; j < numAP; j++){
                    string mac = points->at(i)->getMAC(j).toStdString();
                    int k = 0;
                    for(;k < numAP; k++){
                        if(mac == ap[k].address){
                            break;
                        }
                    }
                    int aux = points->at(i)->getStrength(j) - ap[k].strength;
                    aux = pow(aux, 2);
                    sum += aux;
                }
                d[i] = sqrt(sum);
                if(d[min] > d[i]){
                    //cout << d[min] << "-" << d[i] << endl;
                    min = i;
                }
            }else{
                d[i] = 999999999999;
            }

        }
        for(int i = 0; i < points->count(); i++){
            cout << i << "-" << d[i] << endl;
        }
        cout << min << endl;
        localizationGui->updateUserPosition(points->at(min)->getLogX(), points->at(min)->getLogY());
        cout << points->at(min)->getLogX() << "  " << points->at(min)->getLogY() << flush;


   // }
   //return 0;
}

void LocalizationWidget::on_localization_button_clicked()
{
        calculatePosition();

}
