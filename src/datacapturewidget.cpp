#include <QString>
#include <QThread>
#include <QFileDialog>

#include <string>
#include <sstream>

#include "datacapturewidget.h"
#include "ui_datacapturewidget.h"

#include "mainwindow.h"
#include "datacaptureguiwidget.h"
#include "configuration.h"
#include "point.h"

#include "receiver.h"

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
    QObject::connect(ui->removePoint, SIGNAL(clicked()), this, SLOT(removeLastPoint()));
    QObject::connect(ui->menu, SIGNAL(clicked()), this, SLOT(back()));
    Receiver r;
    QList<QString> devs = r.getAvailableDevices();
    for(int i = 0; i < devs.size(); i++){
        ui->devices->addItem(devs[i]);
    }

}

void DataCaptureWidget::back(){

    mainwindow->switchToAPConfigurationWidget();
}

void DataCaptureWidget::removeLastPoint(){
    if(config->getPoints()->size() > 0){
        config->getPoints()->pop_back();
        dataCaptureGui->updateView();
    }
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
        QString filepath = QFileDialog::getSaveFileName(this,"Save as","", "Text (*.txt)");
        config->setFilePath(filepath);
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
        string d = ui->devices->currentText().toStdString();
        Receiver r(d);
        Direction dir;
        if(ui->North->isChecked()){
            dir = NORTH;
        }else if(ui->East->isChecked()){
            dir = EAST;
        }else if(ui->South->isChecked()){
            dir = SOUTH;
        }else if(ui->West->isChecked()){
            dir = WEST;
        }else{
            //show ERROR
            capturing = false;
            ui->save_button->setEnabled(true);
            return;
        }
        Measurement m(dir, QTime().currentTime());
        for(int i = 0; i < ui->spinBox->value(); i++){
            QList<rssi> aux = r.measureRSSI();
            for(int i = 0; i < aux.size(); i++){
                m.addData(aux[i]);
                if(!mainwindow->getConfig()->isAP(AccessPoint(aux[i].getMAC(), aux[i].getESSID()))){
                    mainwindow->getConfig()->getAccessPoints()->append(new AccessPoint(aux[i].getMAC(), aux[i].getESSID()));
                }
            }
            QThread::msleep(100);
        }
        if(m.getList().size() != 0){
            selectedPoint->addData(m);
        }
        setSelectedPoint(selectedPoint);
        capturing = false;
        ui->save_button->setEnabled(true);
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
    int rowCount = 0;
    for (int j=0; j< dataPoints; j++){
        for(int k = 0; k < p->getMeasurement(j).getList().size(); k++){
            ui->data_table->insertRow(rowCount);

            ui->data_table->setItem(rowCount,2, new QTableWidgetItem(QString::number(p->getMeasurement(j).getStrength(k)), 0));
            ui->data_table->setItem(rowCount,1, new QTableWidgetItem(p->getMeasurement(j).getESSID(k), 0));
            ui->data_table->setItem(rowCount,0, new QTableWidgetItem(p->getMeasurement(j).getTime(), 0));
            rowCount++;
        }

        //ui->data_table->item(row,0)->setFlags(Qt::ItemIsEnabled);
    }


}

bool DataCaptureWidget::getCaptureState(){
    return capturing;
}
