#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configuration.h"
#include "accesspoint.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    APCWidget = new AccessPointConfigurationWidget(this);
    localizationWidget = new LocalizationWidget(this);
    mainWidget = new MainWidget(this);
    newOrLoadWidget = new NewOrLoadWidget(this);
    dataCaptureWidget = new DataCaptureWidget(this);



    this->setCentralWidget(mainWidget);

    ui->exit_action->connect(ui->exit_action, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchToMainWidget(){
    this->setCentralWidget(mainWidget);
}

void MainWindow::switchToAPConfigurationWidget(){
    this->setCentralWidget(APCWidget);
}

void MainWindow::switchToLocalizationWidget(){
    localizationWidget->setConfiguration(config); //this is commented because we dont have a config to load from file yet
    this->setCentralWidget(localizationWidget);
}

void MainWindow::switchToNewOrLoadWidget(){
    this->setCentralWidget(newOrLoadWidget);
}

void MainWindow::switchToDataCaptureWidget(){
    dataCaptureWidget->setConfiguration(config);
    this->setCentralWidget(dataCaptureWidget);
}

void MainWindow::newConfigurationFromData(int w, int l, int spc, QString filePath, QList<AccessPoint*> list){
    config = new Configuration(w,l,spc,filePath, list);
}

void MainWindow::newConfigurationFromData(QString filePath, QList<AccessPoint*> list){
    config = new Configuration(filePath, list);
}

void MainWindow::newConfigurationFromFile(QString filePath){
    config = new Configuration(filePath);
}
