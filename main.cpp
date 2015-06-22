#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

int main(int argc, char *argv[])
{
//    QPluginLoader loader("/home/tzh/Qt/5.4/gcc_64/plugins/sqldrivers/libqsqlmysql.so");
//    loader.load();
//    qDebug() << loader.errorString();
    QApplication a(argc, argv);
    DatabaseAccess db;
    LoginDialog login(&db);
    if (login.exec()){
        MainWindow w;
        w.show();
    return a.exec();
    }
    return 0;
}
