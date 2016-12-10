#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <fstream>
#include "string.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->setSizeGripEnabled(false);
    ui->statusBar->showMessage("Waiting for connection");
    ui->statusBar->setStyleSheet("border-top:1px solid black;background-color:grey; color:black;");

    ui->statusBar->showMessage("Ready to go");

    ui->startButton->setEnabled(true);
    conf.append(-80);
    conf.append(25);

    sock = new QTcpSocket(this);


    int count = 0;
    double baseTime = 0;
    double level = 0;

    customPlot = ui->widget;
    customPlot->addGraph();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::newData()
{
    ControlConfig CC;

    while (sock->bytesAvailable() > 0)
    {
        DS >> CC;
        customPlot->graph(0)->addData(CC.getParams()[15]/1000,CC.getParams()[3]);
        customPlot->rescaleAxes();
        customPlot->replot();
        double* currVals = new double[16];
        for (int i=0;i<16;++i)
            currVals[i] = CC[i];
        sensVals.push_back(currVals);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_savePlotButton_clicked()
{
    customPlot->savePng("phi.png");
    ofstream out("out.txt");
    if (out.is_open())
    {
        for (int i=0;i<sensVals.size();++i)
        {
            for (int j=0;j<16;++j)
                out << sensVals[i][j] << "\t";
            out << endl;
        }
        out.close();
        ui->statusBar->showMessage("File saved");
    }
    else
        ui->statusBar->showMessage("File failed to open");
}

void MainWindow::onSocketConnected()
{
    ui->connectButton->setStyleSheet("background-color:red;color:white;");
    ui->connectButton->setText("Disconnect");
    ui->statusBar->showMessage("Connection established");
}

void MainWindow::onSocketDisconnected()
{
    ui->connectButton->setStyleSheet("background-color:green;color:black;");
    ui->connectButton->setText("Connect");
    ui->statusBar->showMessage("Socket disconnected");

}

void MainWindow::on_connectButton_clicked()
{
    QString addrStr = ui->lineEdit->text();
    sock = new QTcpSocket();
    DS.setDevice(sock);
    connect(sock,SIGNAL(readyRead()),this,SLOT(newData()));
    connect(sock,SIGNAL(connected()),this,SLOT(onSocketConnected()));
    connect(sock,SIGNAL(disconnected()),this,SLOT(onSocketDisconnected()));
    sock->connectToHost(QHostAddress(addrStr),2016);
}

QDataStream& operator>>(QDataStream& str, ControlType& ct) {
  unsigned int type = 0;
  str >> type;
  ct = static_cast<ControlType>(type);
  return str;
}

QDataStream& operator>>( QDataStream& d, ControlConfig& ct ) {
    d  >> ct.type >> ct.params;
    return d;
}

QDataStream& operator<<( QDataStream& d, const ControlConfig& ct ) {
    d << ct.getType() << ct.getParams();
    return d;
}

void MainWindow::on_clearPlotButton_clicked()
{
    customPlot->graph(0)->data().clear();
    customPlot->replot();
}

void MainWindow::on_startButton_clicked()
{
    static bool buttonState = false;
    if (sock->state() == QAbstractSocket::ConnectedState)
    {
        if (!buttonState)
        {
            ControlConfig cush(cnst);
            int val = ui->cushionSpinBox->value();
            val = (double)val/100 * 160 - 80;
            cush.append(val);
            cush.append(val);
            DS << cush;
            ui->startButton->setStyleSheet("background-color:green;color:black;");
            ui->startButton->setText("Stop");
            buttonState = !buttonState;
        }
        else
        {
            ControlConfig motors(manual);
            motors.append(-80);
            motors.append(25);
            DS << motors;

            ControlConfig cush(cnst);
            cush.append(-80);
            cush.append(-80);
            DS << cush;

            ui->startButton->setStyleSheet("background-color:red;color:white;");
            ui->startButton->setText("Start");
            buttonState = !buttonState;
        }
    }
    else
        ui->statusBar->showMessage("No connection");
}

void MainWindow::on_thrustSlider_valueChanged(int value)
{
    conf[0] = value;
    DS << conf;
}

void MainWindow::on_rudderSlider_valueChanged(int value)
{
    conf[1] = value;
    DS << conf;
}
