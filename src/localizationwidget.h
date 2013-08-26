#ifndef LOCALIZATIONWIDGET_H
#define LOCALIZATIONWIDGET_H

#include <QWidget>
#include <QFuture>

#include "receiver.h"
#include "pointprocessed.h"

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
    void setConfiguration(Configuration* c, int aux);
    void calculatePosition();

private slots:
    void measure();
    void realTime();
private:
    string directionToString(Direction d);
    Ui::LocalizationWidget *ui;
    MainWindow* mainwindow;
    LocalizationGuiWidget* localizationGui;
    Configuration* config;
    bool capturing;
    Receiver r;
    QList<PointProcessed> points;
    bool realTimeOn;
    QTimer* timer;
};

#endif // LOCALIZATIONWIDGET_H
