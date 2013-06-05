#ifndef ACCESSPOINTCONFIGURATIONWIDGET_H
#define ACCESSPOINTCONFIGURATIONWIDGET_H

#include <QWidget>
#include <QList>

class MainWindow;
class AccessPoint;

namespace Ui {
    class AccessPointConfigurationWidget;
}

class AccessPointConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccessPointConfigurationWidget(QWidget *parent = 0);
    AccessPointConfigurationWidget(MainWindow *mw);
    ~AccessPointConfigurationWidget();
    void addMAC(QString mac);

private slots:
    void on_selectfile_button_clicked();

    void on_finish_button_clicked();

    void on_add_accesspoint_button_clicked();

private:
    Ui::AccessPointConfigurationWidget *ui;
    MainWindow *mainwindow;
    QString filepath;
    QList<AccessPoint*> apList;

    void addNewRow(QString mac);
    bool tableContainsMAC(QString mac);
    void accessPointConfiguration(int row);

};

#endif // ACCESSPOINTCONFIGURATIONWIDGET_H


