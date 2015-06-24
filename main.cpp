#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    DatabaseAccess db;
    LoginDialog login(&db);
    if (login.exec()){
        MainWindow w(&db);
        w.show();
        return a.exec();
    }
    return 0;
}
