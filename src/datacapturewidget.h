#ifndef DATACAPTUREWIDGET_H
#define DATACAPTUREWIDGET_H

#include <QWidget>
#include <QTimer>

class MainWindow;
class DataCaptureGuiWidget;
class Configuration;
class Point;

namespace Ui {
    class DataCaptureWidget;
}

class DataCaptureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataCaptureWidget(QWidget *parent = 0);
    DataCaptureWidget(MainWindow* mw);
    ~DataCaptureWidget();
    void setConfiguration(Configuration* c);
    void setSelectedPoint(Point* p);
    bool getCaptureState();

private slots:
    void on_save_button_clicked();
    void timerExpired();
    void on_startstop_button_clicked();
    void removeLastPoint();
    void back();

private:
    Ui::DataCaptureWidget *ui;
    MainWindow* mainwindow;
    DataCaptureGuiWidget* dataCaptureGui;
    Configuration* config;
    bool capturing;
    Point* selectedPoint;
    QTimer* timer;
    bool allowSave;
};

#endif // DATACAPTUREWIDGET_H
