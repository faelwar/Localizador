#ifndef DATACAPTUREGUIWIDGET_H
#define DATACAPTUREGUIWIDGET_H

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QVarLengthArray>
#include <QList>

class DataCaptureWidget;
class Configuration;
class Point;
class AccessPoint;

namespace Ui {
    class DataCaptureGuiWidget;
}

class DataCaptureGuiWidget : public QWidget
{
    //Q_OBJECT

public:
    explicit DataCaptureGuiWidget(QWidget *parent = 0);
    ~DataCaptureGuiWidget();
    DataCaptureGuiWidget(DataCaptureWidget* dcw);
    DataCaptureGuiWidget(DataCaptureWidget* dcw, QWidget *parent = 0);

    void setConfiguration(Configuration* c);
    void setAccessPointList(QList<AccessPoint *>* apl);

public slots:
    void updateView();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent (QMouseEvent * event);

private:
    void drawAccessPoint(int x, int y, QPainter* painter);
    void drawGridPoint(int x, int y, QPainter* painter);
    void drawSelectedGridPoint(int x, int y, QPainter* painter);
    double distance(int x1, int y1, int x2, int y2);



    Ui::DataCaptureGuiWidget *ui;
    DataCaptureWidget* captureWidget;
    Configuration* config;
    Point* selectedPoint;
    QVarLengthArray<Point*>* pointList;
    QList<AccessPoint*>* apList;
};

#endif // DATACAPTUREGUIWIDGET_H
