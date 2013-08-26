#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QFileDialog>

#include "mainwindow.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

MainWidget::MainWidget(MainWindow* mw) :
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    mainwindow = mw;
}

void MainWidget::on_offlinemode_button_clicked()
{
    mainwindow->switchToNewOrLoadWidget();
}

void MainWidget::on_onlinemode_button_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Open File","","Text (*.pos)" );

    if (filePath.isEmpty() == false){
        //mainwindow->getConfig()->setFilePath(filePath);
        mainwindow->newConfigurationFromFile(filePath);
        mainwindow->switchToLocalizationWidget(0);
    }
}
