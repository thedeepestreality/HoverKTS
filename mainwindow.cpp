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
    ui->connectButton->setStyleSheet("background-color:red;color:white;");
    ui->connectButton->setText("Disconnect");

//    ui->stopButton->setStyleSheet("background-color:red;color:white;");
//    ui->stopButton->setText("STOP!");
    ui->stopButton->setEnabled(true);

    ifstream fin("pid2_-1.txt");

    int count = 0;
    double baseTime = 0;
    double level = 0;
    QVector<double> x, y,z,lvl;
    while (!fin.eof())
    {
        //str = ts->readLine();
       // QStringList phiStr = str.split(" ");
        char str[128];
        fin.getline(str,128,'\n');
        string strArr(str);
        if (strArr.size() > 0)
        {

            QString qstr = QString::fromStdString(strArr);
            QStringList splitted = qstr.split("\t");
            QString phiStr = splitted[0];
            QString wyStr = splitted[3];
            QString timeStr = splitted[15];
            double phi = phiStr.toDouble();
            double wy = wyStr.toDouble();
            double time = timeStr.toDouble()/1000;
            if (++count==1)
            {
                //++count;
                baseTime = time;
                level = phi;
            }
           // if (count == 250)
             //   break;
            time-=baseTime;
            x.push_back(time);
            y.push_back(phi);
            z.push_back(wy);
            lvl.push_back(level);
        }

    }
    qDebug() << "End!";



    customPlot = ui->widget;
    bool plot = true;
    if (plot)
    {
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y);
        customPlot->graph(0)->setName("Phi");
        customPlot->graph(0)->setPen(QPen(QBrush(QColor(0,0,255)),2));

//        customPlot->addGraph();
//        customPlot->graph(1)->setData(x, z);
//        customPlot->graph(1)->setName("Wy");
//        customPlot->graph(1)->setPen(QPen(QBrush(QColor(0,164,0)),2));

//        customPlot->addGraph();
//        customPlot->graph(1)->setData(x, lvl);
//        customPlot->graph(1)->setName("Level");
//        customPlot->graph(1)->setPen(QPen(QBrush(QColor(164,0,0)),2));

    //    // give the axes some labels:
        customPlot->xAxis->setLabel("Time,s");
        customPlot->rescaleAxes();
        customPlot->yAxis->setRange(15,23);
        customPlot->legend->setVisible(true);
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_savePlotButton_clicked()
{
    customPlot->savePng("phi.png");
}
