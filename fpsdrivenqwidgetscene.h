#ifndef FPSDRIVENQWIDGETSCENE_H
#define FPSDRIVENQWIDGETSCENE_H

#include <QWidget>

class FpsDrivenQWidgetScene : public QWidget
{
    Q_OBJECT
public:
    explicit FpsDrivenQWidgetScene(QWidget *parent = nullptr);

public:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

};

#endif // FPSDRIVENQWIDGETSCENE_H
