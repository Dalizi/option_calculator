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
    logindialog.cpp \
    addorderdialog.cpp \
    tradetypes.cpp \
    transactionviewdialog.cpp \
    valuation_class.cpp \
    accessredis.cpp \
    base_function.cpp

HEADERS  += mainwindow.h \
    databaseaccess.h \
    logindialog.h \
    addorderdialog.h \
    tradetypes.h \
    transactionviewdialog.h \
    base_function.h \
    valuation_class.h \
    accessredis.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    addorderdialog.ui \
    transactionviewdialog.ui

RESOURCES += \
    multi_language.qrc

LIBS += -lhiredis
