#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWindow;

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    MainWidget(MainWindow* mw);

    ~MainWidget();

private slots:
    void on_offlinemode_button_clicked();

    void on_onlinemode_button_clicked();

private:
    Ui::MainWidget *ui;
    MainWindow* mainwindow;
    QString filePath;
};

#endif // MAINWIDGET_H
