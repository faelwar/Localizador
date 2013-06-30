#ifndef LOCALIZATIONGUIWIDGET_H
#define LOCALIZATIONGUIWIDGET_H

#include <QWidget>
#include <QVarLengthArray>
#include <QFuture>
#include "receiver.h"
#include <string>
#include <math.h>
using namespace std;

class Configuration;
class Point;

namespace Ui {
    class LocalizationGuiWidget;
}

class LocalizationGuiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LocalizationGuiWidget(QWidget *parent = 0);
    ~LocalizationGuiWidget();

    void setConfiguration(Configuration* c);
    void updateUserPosition(int x, int y);


public slots:
    void updateView();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawGridPoint(int x, int y, QPainter* painter);
    void drawUser(int x, int y, QPainter* painter);

    Ui::LocalizationGuiWidget *ui;
    Configuration* config;
    QVarLengthArray<Point*>* pointList;
    int userGuiX;
    int userGuiY;
    int userX;
    int userY;

};

#endif // LOCALIZATIONGUIWIDGET_H
