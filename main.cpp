#include "mainwindow.h"
#include "logindialog.h"
#include "valuation_class.h"

#include <fstream>

#include <QApplication>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>

using namespace std;

string REDIS_ADDR = "127.0.0.1";
int REDIS_PORT = 6379;
string REDIS_PASSWD = "";

int getRedisInfo() {
    ifstream redis_info("redis_info.ini");
    if (!redis_info.is_open()) {
        QMessageBox::warning(0, "Warning", "读取redis连接配置文件失败。");
        exit(1);
    }
    string line;
    if (getline(redis_info, line)) REDIS_ADDR = line;
    if (getline(redis_info, line)) REDIS_PORT = stod(line);
    if (getline(redis_info, line)) REDIS_PASSWD = line;
    return 0;
}

int main(int argc, char *argv[])
{
    getRedisInfo();
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
