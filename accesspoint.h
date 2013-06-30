#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class AccessPoint
{
public:
    AccessPoint();
    AccessPoint(QString m, QString e);
    AccessPoint(QString m, int lx, int ly);
    AccessPoint(const AccessPoint& a);
    AccessPoint operator=(AccessPoint a);
    QString getMAC();
    QString getESSID();
    int getlogX();
    int getlogY();
    int getguiX();
    int getguiY();
    void setguiX(int x);
    void setguiY(int y);
    void writeToFile(QXmlStreamWriter* xmlWriter);
    void readFromFile(QXmlStreamReader* xmlReader);

private:
    QString mac;
    QString ESSID;
    int logX;
    int logY;
    int guiX;
    int guiY;
};

#endif // ACCESSPOINT_H
