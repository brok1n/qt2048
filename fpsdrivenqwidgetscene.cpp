#include "fpsdrivenqwidgetscene.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>
#include <QKeyEvent>

FpsDrivenQWidgetScene::FpsDrivenQWidgetScene(QWidget *parent) : QWidget(parent)
{

}

void FpsDrivenQWidgetScene::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << "FpsDrivenQWidgetScene::keyPressEvent:" << event->text() << " : " << event->key() << " : " << event->type();
}

void FpsDrivenQWidgetScene::paintEvent(QPaintEvent *event)
{

}
