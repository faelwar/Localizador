#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include "accesspointconfigurationwidget.h"
#include "localizationwidget.h"
#include "mainwidget.h"
#include "neworloadwidget.h"
#include "datacapturewidget.h"

class Configuration;
class AccessPoint;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void switchToMainWidget();
    void switchToAPConfigurationWidget();
    void switchToLocalizationWidget();
    void switchToNewOrLoadWidget();
    void switchToDataCaptureWidget();
    void newConfigurationFromData(int w, int l, int spc, QString filePath, QList<AccessPoint*> list);//this will need a AP listing too
    void newConfigurationFromFile(QString filePath);//will read all existing configuration parameters from file
    void newConfigurationFromData(QString filePath, QList<AccessPoint*> list);

public slots:
    //void previous();

private:
    Ui::MainWindow *ui;
    AccessPointConfigurationWidget* APCWidget;
    LocalizationWidget* localizationWidget;
    NewOrLoadWidget* newOrLoadWidget;
    MainWidget* mainWidget;
    DataCaptureWidget* dataCaptureWidget;
    Configuration* config;
};

#endif // MAINWINDOW_H
