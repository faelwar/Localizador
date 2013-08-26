#include "localizationguiwidget.h"
#include "ui_localizationguiwidget.h"
#include "configuration.h"
#include "point.h"

#include <QPainter>
#include <QFuture>

LocalizationGuiWidget::LocalizationGuiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalizationGuiWidget)
{
    ui->setupUi(this);
    userEuclideanX = -1;
    this->setStyleSheet("background-image: url(:/new/prefix1/dpi);background-repeat: none;");

}

LocalizationGuiWidget::~LocalizationGuiWidget()
{
    delete ui;
}

void LocalizationGuiWidget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();
    //DRAW

    int numberOfPoints = config->getNumberOfPoints();
    for(int i = 0; i < numberOfPoints; i++){
        drawGridPoint(pointList->at(i)->getGuiX(), pointList->at(i)->getGuiY(), &painter, QColor(Qt::gray));
    }
    int numberOfAP = config->getAccessPoints()->size();
    for(int i = 0; i < numberOfAP; i++){
        drawGridPoint(config->getAccessPoints()->at(i)->getguiX(), config->getAccessPoints()->at(i)->getguiY(), &painter, QColor(255,128,0,255));
    }

    if(userEuclideanX != -1){
        drawUser(userManhantanX,userManhantanY, &painter, QColor(Qt::blue));
        drawUser(userEuclideanX,userEuclideanY, &painter, QColor(Qt::red));
        drawUser(userMClosestMX,userMClosestMY, &painter, QColor(Qt::green));
        drawUser(userMClosestEX,userMClosestEY, &painter, QColor(Qt::cyan));
    }

    //END DRAW
    painter.restore();
}

void LocalizationGuiWidget::drawGridPoint(int x, int y, QPainter* painter, QColor color){ //this function should be private
    int radius = 5;

    QPen pen = QPen(color);
    QBrush brush = QBrush(color);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(QPoint(x,y), radius, radius);
}

void LocalizationGuiWidget::drawUser(int x, int y, QPainter* painter, QColor color){ //this function should be private

    int radius = 5;

    QPen pen = QPen(color);
    pen.setWidth(3);
    QBrush brush = QBrush(color);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(x,y,radius,radius);

}

void LocalizationGuiWidget::updateView(){

    this->repaint();
}

void LocalizationGuiWidget::setConfiguration(Configuration* c){
    config = c;
    pointList = config->getPoints();
    //updateUserPosition(15,7);
}

void LocalizationGuiWidget::updateUserPositionManhantan(int x, int y, Direction d){
    //this->repaint();
    userManhantanX = x;
    userManhantanY = y;
    dirManhantan = d;
    updateView();
}


void LocalizationGuiWidget::updateUserPositionEuclidean(int x, int y, Direction d){
    //this->repaint();
    userEuclideanX = x;
    userEuclideanY = y;
    dir = d;
    updateView();
}


void LocalizationGuiWidget::updateUserMClosestEuclideanPosition(int x, int y){
    //this->repaint();
    userMClosestEX = x;
    userMClosestEY = y;
    updateView();
}

void LocalizationGuiWidget::updateUserMClosestManhattanPosition(int x, int y){
    //this->repaint();
    userMClosestMX = x;
    userMClosestMY = y;
    updateView();
}

