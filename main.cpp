#include "mainwindow.h"
#include "logindialog.h"
#include "valuation_class.h"

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

using namespace std;

string REDIS_ADDR = "127.0.0.1";
int REDIS_PORT = 6379;
string REDIS_PASSWD = "";

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    DatabaseAccess db;
//    Option_Value calc_server("ConfigFile.txt", );
    LoginDialog login(&db);
    if (login.exec()){
        db.test();
        MainWindow w(&db);
        w.show();
        return a.exec();
    }
    return 0;
}
