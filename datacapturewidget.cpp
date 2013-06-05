#include <QString>
#include <string>
#include <sstream>

#include "datacapturewidget.h"
#include "ui_datacapturewidget.h"

#include "mainwindow.h"
#include "datacaptureguiwidget.h"
#include "configuration.h"
#include "point.h"

#include "wlan.h"

using namespace std;

DataCaptureWidget::DataCaptureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataCaptureWidget)
{
    ui->setupUi(this);

}

DataCaptureWidget::~DataCaptureWidget()
{
    delete ui;
}

DataCaptureWidget::DataCaptureWidget(MainWindow* mw) :
    ui(new Ui::DataCaptureWidget)
{
    ui->setupUi(this);
    mainwindow = mw;
    dataCaptureGui = new DataCaptureGuiWidget(this,this);
    ui->scrollArea->setWidget(dataCaptureGui);
    dataCaptureGui->show();
    capturing = false;
    allowSave = true;
    selectedPoint = NULL;
}

void DataCaptureWidget::setConfiguration(Configuration* c){
    config = c;

    //do some other things that we need to do since we have the data now.
    int gridspace = config->getGridSpacing();
    int width = config->getWidth();
    int length = config->getLength();
    int spacing = config->getLogicalSpacing();

    dataCaptureGui->resize(2*gridspace + gridspace*(width/spacing), 2*gridspace + gridspace*(length/spacing));
    dataCaptureGui->setConfiguration(config);
    dataCaptureGui->setAccessPointList(config->getAccessPoints());
}

void DataCaptureWidget::timerExpired(){
    ui->message_label->setText("");
    allowSave = true;
}

void DataCaptureWidget::on_save_button_clicked()
{
    if (allowSave == true){
        allowSave = false;
        //make sure we stop capturing data when we do this... or disable the save button while capturing
        config->saveToFile();

        ui->message_label->setText("...Saved...");

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this,SLOT(timerExpired()));
        timer->start(3500); //CHANGED
    }
}

void DataCaptureWidget::on_startstop_button_clicked()
{
    if (capturing == false && selectedPoint != NULL){
        capturing = true;
        ui->save_button->setEnabled(false);
        //QVarLengthArray<Point*>* points = config->getPoints();
        SignalStrength* accessPoints;
        Receiver r;
        if (r.init(1)==OK){
              accessPoints = r.Receive();
              r.shutdown();
        }


        for(int i = 0; i < config->getAccessPoints()->count(); i++ ){
            //int row = ui->data_table->rowCount();
            //get the selected point
            selectedPoint->addData(QString(accessPoints[i].address),QTime(QTime::currentTime()), accessPoints[i].strength);
            setSelectedPoint(selectedPoint);
        }
        //points->append(aux);
    }else{
        capturing = false;
        ui->save_button->setEnabled(true);
    }
}

void DataCaptureWidget::setSelectedPoint(Point* p){
    int rows = ui->data_table->rowCount();
    for (int i = 0; i < rows; i++){
        ui->data_table->removeRow(0);
    }

    selectedPoint = p;
    int dataPoints = p->getNumberOfDataElements();

    //here we can set all the fields for the gui
    ui->x_lineedit->setText(QString::number(p->getLogX()));
    ui->y_lineedit->setText(QString::number(p->getLogY()));

    for (int j=0; j< dataPoints; j++){
        ui->data_table->insertRow(j);
        ui->data_table->setItem(j,2, new QTableWidgetItem(QString::number(p->getStrength(j)), 0));
        ui->data_table->setItem(j,1, new QTableWidgetItem(p->getMAC(j), 0));
        ui->data_table->setItem(j,0, new QTableWidgetItem(p->getTime(j).toString(), 0));

        //ui->data_table->item(row,0)->setFlags(Qt::ItemIsEnabled);
    }


}

bool DataCaptureWidget::getCaptureState(){
    return capturing;
}
