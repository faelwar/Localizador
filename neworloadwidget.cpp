#include "neworloadwidget.h"
#include "ui_neworloadwidget.h"
#include <QFileDialog>

#include "mainwindow.h"

NewOrLoadWidget::NewOrLoadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewOrLoadWidget)
{
    ui->setupUi(this);
}

NewOrLoadWidget::~NewOrLoadWidget()
{
    delete ui;
}

NewOrLoadWidget::NewOrLoadWidget(MainWindow* mw) :
    ui(new Ui::NewOrLoadWidget)
{
    ui->setupUi(this);
    mainwindow = mw;
}

void NewOrLoadWidget::on_load_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select a file","","Text (*.txt)");

    if(filename == NULL){mainwindow->switchToNewOrLoadWidget();}
    else if(filename.contains(".txt", Qt::CaseSensitive)){
        mainwindow->newConfigurationFromFile(filename);
        mainwindow->switchToDataCaptureWidget();
    }
}

void NewOrLoadWidget::on_new_button_clicked()
{
    mainwindow->switchToAPConfigurationWidget();
}
