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


public:
    explicit LocalizationGuiWidget(QWidget *parent = 0);
    ~LocalizationGuiWidget();

    void setConfiguration(Configuration* c);
    void updateUserPositionManhantan(int x, int y, Direction d);
    void updateUserPositionEuclidean(int x, int y, Direction d);
    void updateUserMClosestManhattanPosition(int x, int y);
    void updateUserMClosestEuclideanPosition(int x, int y);


public slots:
    void updateView();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawGridPoint(int x, int y, QPainter* painter, QColor color);
    void drawUser(int x, int y, QPainter* painter, QColor color);

    Ui::LocalizationGuiWidget *ui;
    Configuration* config;
    QVarLengthArray<Point*>* pointList;
    int userManhantanX;
    int userManhantanY;
    int userEuclideanX;
    int userEuclideanY;
    int userMClosestMX;
    int userMClosestMY;
    int userMClosestEX;
    int userMClosestEY;
    Direction dirManhantan;
    Direction dirEuclidean;
    Direction dir;

};

#endif // LOCALIZATIONGUIWIDGET_H
