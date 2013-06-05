#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class AccessPoint
{
public:
    AccessPoint(QString m);
    AccessPoint(QString m, int lx, int ly);
    QString getMAC();
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
    int logX;
    int logY;
    int guiX;
    int guiY;
};

#endif // ACCESSPOINT_H
