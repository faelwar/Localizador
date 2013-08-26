#include "datacaptureguiwidget.h"
#include "ui_datacaptureguiwidget.h"
#include "datacapturewidget.h"
#include "configuration.h"
#include "point.h"
#include "accesspoint.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>



DataCaptureGuiWidget::DataCaptureGuiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataCaptureGuiWidget)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(./images/dpi.png)");



}

DataCaptureGuiWidget::~DataCaptureGuiWidget()
{
    delete ui;
}

DataCaptureGuiWidget::DataCaptureGuiWidget(DataCaptureWidget* dcw) :
    ui(new Ui::DataCaptureGuiWidget)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(./images/dpi.png)");
    captureWidget = dcw;
    selectedPoint = NULL;
}

DataCaptureGuiWidget::DataCaptureGuiWidget(DataCaptureWidget* dcw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataCaptureGuiWidget)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/new/prefix1/dpi);background-repeat: none;");
    captureWidget = dcw;
    selectedPoint = NULL;
}

void DataCaptureGuiWidget::updateView(){
    update();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataCaptureGuiWidget::mousePressEvent (QMouseEvent * event){
    int mouseX;
    int mouseY;

    //system("yay");

    if (event->button() == Qt::LeftButton){

        //system("weeeee");


        mouseX = event->x();
        mouseY = event->y();

        //loop through points, find the closest one, and set it to selected
        //(if we are not capturing data.. have to ask datacapturewidget if we are capturing)
        //need to track current shortest distance and current closest point.

        if (captureWidget->getCaptureState() == false){
            //now we can set the selected point
            //(also check if we click within a certain range.. say 30 pixels)
            double shortestDistance = 100000;
            Point* closestPoint = NULL;
            int numberOfPoints = pointList->size();

            for (int i = 0; i<numberOfPoints; i++){
                if(distance(mouseX,mouseY,pointList->at(i)->getGuiX(),pointList->at(i)->getGuiY()) < shortestDistance){
                    closestPoint = pointList->at(i);
                    shortestDistance = distance(mouseX,mouseY,pointList->at(i)->getGuiX(),pointList->at(i)->getGuiY());
                }
            }

            if(shortestDistance <= 15){
                selectedPoint = closestPoint;
                captureWidget->setSelectedPoint(closestPoint);
            }else{
                pointList->append(new Point(mouseX, mouseY, mouseX, mouseY));

            }

        }
    }
    this->update();
}

void DataCaptureGuiWidget::paintEvent(QPaintEvent * /* event */){

    //QPens draw lines
    //      QPen circlepen = QPen(QColor(Qt::black));
    //      QPen.setWidth(int)
    //QBrushs draw fill
    //      QBrush pointbrush = QBrush(QColor(Qt::darkRed));
    //      QBrush piebrush = QBrush(QColor(90,140,125,120));
    //QPainter specifies what you are painting to
    //      painter.drawPoint(x,y);
    //      painter.setPen(QPen);
    //      painter.setBrush(QBrush);
    //      painter.drawEllipse(towerlist.at(i)->getRectXPos(),towerlist.at(i)->getRectYPos(),towerlist.at(i)->getRadius()*2,towerlist.at(i)->getRadius()*2);
    //      painter.drawLine(X1,Y1,X2,Y2);
    //      painter.drawText(X,Y,QString);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();
    //DRAW

    for (int j = 0; j<apList->size(); j++){
        drawAccessPoint(apList->at(j)->getguiX(), apList->at(j)->getguiY(), &painter);
    }

    int numberOfPoints = config->getNumberOfPoints();
    for(int i = 0; i < numberOfPoints; i++){
        if(selectedPoint == pointList->at(i)){
            drawSelectedGridPoint(pointList->at(i)->getGuiX(), pointList->at(i)->getGuiY(), &painter);
        }else{
            drawGridPoint(pointList->at(i)->getGuiX(), pointList->at(i)->getGuiY(), &painter);
        }
    }

    //END DRAW
    painter.restore();
}

void DataCaptureGuiWidget::drawAccessPoint(int x, int y, QPainter* painter){//this function should be private
    int radius = 20;

    QPen pen = QPen(QColor(255,128,0,255));
    QBrush brush = QBrush(QColor(255,128,0,255));
    QPen pen2 = QPen(QColor(Qt::black));
    QBrush brush2 = QBrush(Qt::NoBrush);
    pen2.setWidth(3);

    painter->setPen(pen);
    painter->setBrush(brush);

    //painter->drawPie(x-radius,y-radius, 2*radius, 2*radius, -45*16, 90*16);
    //painter->drawPie(x-radius,y-radius, 2*radius, 2*radius, 135*16, 90*16);
    painter->drawEllipse(QPoint(x,y), (int)(radius*0.25), (int)(radius*0.25));

    painter->setPen(pen2);
    painter->setBrush(brush2);
    //painter->drawEllipse(QPoint(x,y), (int)(radius*0.8), (int)(radius*0.8));
    //painter->drawEllipse(QPoint(x,y), (int)(radius*0.5), (int)(radius*0.5));

}

void DataCaptureGuiWidget::drawGridPoint(int x, int y, QPainter* painter){ //this function should be private
    int radius = 5;

    QPen pen = QPen(QColor(Qt::gray));
    QBrush brush = QBrush(QColor(Qt::gray));

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(QPoint(x,y), radius, radius);
}

void DataCaptureGuiWidget::drawSelectedGridPoint(int x, int y, QPainter* painter){ //this function should be private
    int radius1 = 5;
    int radius2 = 7;
    int radius3 = 10;

    QPen pen1 = QPen(QColor(Qt::yellow));
    QBrush brush1 = QBrush(QColor(Qt::yellow));

    QPen pen2 = QPen(QColor(Qt::black));
    QBrush brush2 = QBrush(QColor(Qt::black));

    QPen pen3 = QPen(QColor(Qt::yellow));
    QBrush brush3 = QBrush(QColor(Qt::yellow));

    QPen pen4 = QPen(QColor(Qt::black));
    pen4.setWidth(4);
    QBrush brush4 = QBrush(QColor(Qt::black));

    painter->setPen(pen3);
    painter->setBrush(brush3);
    painter->drawEllipse(QPoint(x,y), radius3, radius3);

    painter->setPen(pen2);
    painter->setBrush(brush2);
    painter->drawEllipse(QPoint(x,y), radius2, radius2);

    painter->setPen(pen4);
    painter->setBrush(brush4);
    painter->drawLine(x, y-radius3, x, y+radius3);
    painter->drawLine(x-radius3, y, x+radius3, y);

    painter->setPen(pen1);
    painter->setBrush(brush1);
    painter->drawEllipse(QPoint(x,y), radius1, radius1);
}

void DataCaptureGuiWidget::setConfiguration(Configuration* c){
    config = c;
    pointList = config->getPoints();
}

void DataCaptureGuiWidget::setAccessPointList(QList<AccessPoint *>* apl){
    apList = apl;
}

double DataCaptureGuiWidget::distance(int x1, int y1, int x2, int y2){
    int X = abs(x2 - x1);
    int Y = abs(y2 - y1);

    return sqrt((double)(X*X) + (double)(Y*Y));
}
