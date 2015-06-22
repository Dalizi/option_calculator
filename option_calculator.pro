#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T15:22:34
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = option_calculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    databaseaccess.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    databaseaccess.h \
    logindialog.h

FORMS    += mainwindow.ui \
    logindialog.ui
