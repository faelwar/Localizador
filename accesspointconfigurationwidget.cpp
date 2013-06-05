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

    //for testing
    //addMAC("12:34:56:78:90:AB");
    //addMAC("45:29:13:74:88:AB");
    //addMAC("00:00:00:00:00:AB");
    //addMAC("12:34:56:78:90:AB");
    //addMAC("45:29:13:74:88:AB");

}

void AccessPointConfigurationWidget::on_selectfile_button_clicked()
{
    filepath = QFileDialog::getSaveFileName(this,"Save as","", "Text (*.txt)");
    QString filename = filepath.mid(filepath.lastIndexOf('/')+1, filepath.length() - filepath.lastIndexOf('/'));
    ui->filename_lineedit->setText(filename);
}

void AccessPointConfigurationWidget::on_finish_button_clicked()
{
    if (ui->filename_lineedit->text().isEmpty()){
        ui->error_label->setText("You must specify a file to store the configuration.");
        return;
    }else if(ui->width_lineedit->text().isEmpty()){
        ui->error_label->setText("You must specify a X value for the size of grid.");
        return;
    }else if(ui->length_lineedit->text().isEmpty()){
        ui->error_label->setText("You must specify a Y value for the size of grid.");
        return;
    }else if(ui->spacing_lineedit->text().isEmpty()){
        ui->error_label->setText("You must specify a value for the grid spacing.");
        return;
    }else{
        ui->error_label->setText("");
        //capture filepath and all the data from the table and prepare to move to next widget

        for(int i = 0; i<ui->accesspoint_table->rowCount(); i++){
            accessPointConfiguration(i);
        }


        int x = ui->width_lineedit->text().toInt();
        int y = ui->length_lineedit->text().toInt();
        int spacing = ui->spacing_lineedit->text().toInt();

        mainwindow->newConfigurationFromData(x,y,spacing,filepath, apList);
        mainwindow->switchToDataCaptureWidget();
    }
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
        apList.append(new AccessPoint(ui->accesspoint_table->item(row,0)->text(),
                                      ui->accesspoint_table->item(row,1)->text().toInt(),
                                      ui->accesspoint_table->item(row,2)->text().toInt()));
    }
}

void AccessPointConfigurationWidget::on_add_accesspoint_button_clicked()
{
    addNewRow("");
}
