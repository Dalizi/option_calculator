#include "mainwindow.h"
#include "logindialog.h"
#include "valuation_class.h"

#include <QApplication>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>

using namespace std;

string REDIS_ADDR = "127.0.0.1";
int REDIS_PORT = 6379;
string REDIS_PASSWD = "";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QCoreApplication::libraryPaths();
    DatabaseAccess db;
    LoginDialog login(&db);
    if (login.exec()){
        OptionValue calc_server("TradeDate.txt", db.getParam());
        MainWindow w(&db, &calc_server);
        w.show();
        return a.exec();
    }
    return 0;
}
