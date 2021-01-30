#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "eventdrivenqwidgetscene.h"
#include "fpsdrivenqwidgetscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    EventDrivenQWidgetScene *eventScene;
    FpsDrivenQWidgetScene *fpsScene;
};
#endif // MAINWINDOW_H
