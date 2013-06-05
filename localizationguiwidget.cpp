#include "localizationguiwidget.h"
#include "ui_localizationguiwidget.h"
#include "configuration.h"
#include "point.h"

#include <QPainter>
#include <QFuture>
#include <QtConcurrentRun>

LocalizationGuiWidget::LocalizationGuiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalizationGuiWidget)
{
    ui->setupUi(this);
    userX = -1;
    userY = -1;

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
        drawGridPoint(pointList->at(i)->getGuiX(), pointList->at(i)->getGuiY(), &painter);
    }

    if (userX != -1 && userY != -1){
        drawUser(userGuiX,userGuiY,&painter);
    }

    //END DRAW
    painter.restore();
}

void LocalizationGuiWidget::drawGridPoint(int x, int y, QPainter* painter){ //this function should be private
    int radius = 5;

    QPen pen = QPen(QColor(Qt::gray));
    QBrush brush = QBrush(QColor(Qt::gray));

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(QPoint(x,y), radius, radius);
}

void LocalizationGuiWidget::drawUser(int x, int y, QPainter* painter){ //this function should be private

    int radius = 5;

    QPen pen = QPen(QColor(Qt::blue));
    pen.setWidth(3);
    QBrush brush = QBrush(QColor(Qt::blue));
    QPen pen1 = QPen(QColor(255,128,0,255));
    pen1.setWidth(5);
    QBrush brush1 = QBrush(QColor(255,128,0,255));
    QPen fontpen = QPen(QColor(Qt::black));

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(x, y-20, 80, 12);

    painter->setPen(fontpen);
    QString coordinates = "  X= ";
    coordinates.append(QString::number(userX));
    coordinates.append(" , Y= ");
    coordinates.append(QString::number(userY));
    painter->drawText(x, y-10,coordinates);

    painter->setPen(pen1);
    painter->setBrush(brush1);
    painter->drawLine(x-radius,y-radius,x+radius,y+radius);
    painter->drawLine(x-radius,y+radius,x+radius,y-radius);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawLine(x-radius,y-radius,x+radius,y+radius);
    painter->drawLine(x-radius,y+radius,x+radius,y-radius);
}

void LocalizationGuiWidget::updateView(){
    update();
}

void LocalizationGuiWidget::setConfiguration(Configuration* c){
    config = c;
    pointList = config->getPoints();
    //updateUserPosition(15,7);
}

void LocalizationGuiWidget::updateUserPosition(int x, int y){

    userX = x;
    userY = y;

    int spacing = config->getLogicalSpacing();
    int gridspacing = config->getGridSpacing();

    userGuiX = (int)(gridspacing + gridspacing*(double)((double)userX / (double)spacing));
    userGuiY = (int)(gridspacing + gridspacing*(double)((double)userY / (double)spacing));
    updateView();
}


