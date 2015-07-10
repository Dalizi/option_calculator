#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T15:22:34
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11
DEFINES += NOMINMAX USE_DL_PREFIX LACKS_STDLIB_H NO_QFORKIMPL WIN32_IOCP

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
    base_function.cpp \
    optioncalcdialog.cpp \
    positiontablemodel.cpp \
    adduserdialog.cpp \
    resetpassworddialog.cpp \
    paramsettingdialog.cpp \
    addparamdialog.cpp

HEADERS  += mainwindow.h \
    databaseaccess.h \
    logindialog.h \
    addorderdialog.h \
    tradetypes.h \
    transactionviewdialog.h \
    base_function.h \
    valuation_class.h \
    accessredis.h \
    optioncalcdialog.h \
    positiontablemodel.h \
    adduserdialog.h \
    resetpassworddialog.h \
    paramsettingdialog.h \
    addparamdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    addorderdialog.ui \
    transactionviewdialog.ui \
    optioncalcdialog.ui \
    adduserdialog.ui \
    resetpassworddialog.ui \
    paramsettingdialog.ui \
    addparamdialog.ui

RESOURCES += \
    multi_language.qrc

INCLUDEPATH += C:\Users\mystconqueror\Downloads\redis-release-2.8.17\redis-release-2.8.17\deps

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lhiredis
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lhiredis

INCLUDEPATH += $$PWD/Debug
DEPENDPATH += $$PWD/Debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lWin32_Interop
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lWin32_Interop

INCLUDEPATH += $$PWD/Debug
DEPENDPATH += $$PWD/Debug
