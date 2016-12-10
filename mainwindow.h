#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>
#include "controlconfig.h"
#include "customjoystick.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_savePlotButton_clicked();


    void on_connectButton_clicked();

    void on_clearPlotButton_clicked();

    void on_startButton_clicked();

    void on_thrustSlider_valueChanged(int value);

    void on_rudderSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    QTimer* testTimer;
    QTcpSocket* sock;
    QDataStream DS;
    ControlConfig conf;
    QList<double*> sensVals;

public slots:
    void newData();
    void onSocketConnected();
    void onSocketDisconnected();
};

#endif // MAINWINDOW_H
