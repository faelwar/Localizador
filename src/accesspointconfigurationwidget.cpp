#include "accesspointconfigurationwidget.h"
#include "ui_accesspointconfigurationwidget.h"
#include <QFileDialog>

#include "mainwindow.h"
#include "accesspoint.h"

AccessPointConfigurationWidget::AccessPointConfigurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccessPointConfigurationWidget)
{
    ui->setupUi(this);
    cout << "OI" << apList->length() << endl;
}

AccessPointConfigurationWidget::~AccessPointConfigurationWidget()
{
    delete ui;
}

AccessPointConfigurationWidget::AccessPointConfigurationWidget(MainWindow* mw) :
    ui(new Ui::AccessPointConfigurationWidget)
{
    ui->setupUi(this);
    mainwindow = mw;
    QObject::connect(ui->finish, SIGNAL(clicked()), this, SLOT(finish()));

}

void AccessPointConfigurationWidget::finish(){
    for(int i = 0; i < ui->accesspoint_table->rowCount(); i++){
        apList->at(i)->setguiX(ui->accesspoint_table->item(i,1)->text().toInt());
        apList->at(i)->setguiY(ui->accesspoint_table->item(i,2)->text().toInt());
    }
    if(config->getFilePath() == ""){
        QString filepath = QFileDialog::getSaveFileName(this,"Save as","", "Text (*.pre)");
        config->setFilePath(filepath);
    }
    config->saveToFile();
    mainwindow->switchToLocalizationWidget(1);
}

void AccessPointConfigurationWidget::setApList(QList<AccessPoint*>* list){
    apList = list;
}

void AccessPointConfigurationWidget::loadApList(){
    for(int i = 0; i < apList->length(); i++){
        ui->accesspoint_table->insertRow(i);
        QTableWidgetItem *item = new QTableWidgetItem(apList->at(i)->getESSID(), 0);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->accesspoint_table->setItem(i,0, item);

        if(apList->at(i)->getguiX() < 0){
            ui->accesspoint_table->setItem(i,1, new QTableWidgetItem("", 0));
        }else{
            ui->accesspoint_table->setItem(i,1, new QTableWidgetItem(QString::number(apList->at(i)->getguiX()), 0));
        }
        if(apList->at(i)->getguiY() < 0){
            ui->accesspoint_table->setItem(i,2, new QTableWidgetItem("", 0));
        }else{
            ui->accesspoint_table->setItem(i,2, new QTableWidgetItem(QString::number(apList->at(i)->getguiY()), 0));
        }
    }
}

void AccessPointConfigurationWidget::setConfig(Configuration* c){
    config = c;
}

void AccessPointConfigurationWidget::on_selectfile_button_clicked()
{
    filepath = QFileDialog::getSaveFileName(this,"Save as","", "Text (*.txt)");
    QString filename = filepath.mid(filepath.lastIndexOf('/')+1, filepath.length() - filepath.lastIndexOf('/'));

}


void AccessPointConfigurationWidget::addMAC(QString mac){//public
    if (!tableContainsMAC(mac)){addNewRow(mac);}
}

void AccessPointConfigurationWidget::addNewRow(QString mac){//private
    int row = ui->accesspoint_table->rowCount();
    ui->accesspoint_table->insertRow(row);
    ui->accesspoint_table->setItem(row,0, new QTableWidgetItem(mac, 0));
    ui->accesspoint_table->setItem(row,1, new QTableWidgetItem("", 0));
    ui->accesspoint_table->setItem(row,2, new QTableWidgetItem("", 0));
    //ui->accesspoint_table->item(row,0)->setFlags(Qt::ItemIsEnabled);
}

bool AccessPointConfigurationWidget::tableContainsMAC(QString mac){//private
    for (int y = 0; y<ui->accesspoint_table->rowCount(); y++){
        if (ui->accesspoint_table->item(y,0)->text() == mac){
            return true;
        }
    }
    return false;
}

void AccessPointConfigurationWidget::accessPointConfiguration(int row){//private
    //if both x and y for the row have a value
    if(!ui->accesspoint_table->item(row,1)->text().isEmpty() && !ui->accesspoint_table->item(row,2)->text().isEmpty()){
        //then create a new AP from that data and add it to the list
        apList->append(new AccessPoint(ui->accesspoint_table->item(row,0)->text(),
                                      ui->accesspoint_table->item(row,1)->text().toInt(),
                                      ui->accesspoint_table->item(row,2)->text().toInt()));
    }
}

void AccessPointConfigurationWidget::on_add_accesspoint_button_clicked()
{
    addNewRow("");
}
