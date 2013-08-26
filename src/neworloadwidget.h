#ifndef NEWORLOADWIDGET_H
#define NEWORLOADWIDGET_H

#include <QWidget>

class MainWindow;

namespace Ui {
    class NewOrLoadWidget;
}

class NewOrLoadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewOrLoadWidget(QWidget *parent = 0);
    NewOrLoadWidget(MainWindow* mw);
    ~NewOrLoadWidget();

private slots:
    void on_load_button_clicked();

    void on_new_button_clicked();

private:
    Ui::NewOrLoadWidget *ui;
    MainWindow* mainwindow;
};

#endif // NEWORLOADWIDGET_H
