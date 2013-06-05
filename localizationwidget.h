#ifndef LOCALIZATIONWIDGET_H
#define LOCALIZATIONWIDGET_H

#include <QWidget>
#include <QFuture>

#include "wlan.h"

class MainWindow;
class Configuration;
class LocalizationGuiWidget;

namespace Ui {
    class LocalizationWidget;
}

class LocalizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LocalizationWidget(QWidget *parent = 0);
    LocalizationWidget(MainWindow* mw);
    ~LocalizationWidget();
    void setConfiguration(Configuration* c);
    void calculatePosition();

private slots:
    void on_localization_button_clicked();

private:

    Ui::LocalizationWidget *ui;
    MainWindow* mainwindow;
    LocalizationGuiWidget* localizationGui;
    Configuration* config;
    bool capturing;
    Receiver r;
};

#endif // LOCALIZATIONWIDGET_H
