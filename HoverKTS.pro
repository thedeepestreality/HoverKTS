#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T14:55:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = HoverKTS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    controlconfig.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    controlconfig.h

FORMS    += mainwindow.ui
