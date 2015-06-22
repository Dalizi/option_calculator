#include "databaseaccess.h"

#include <QDebug>

using namespace std;

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3309);  //设置端口
    db.setDatabaseName("mysql");  //设置数据库名称
    if(!db.open())
        qDebug()<<"failed to connect to mysql";
    else
        qDebug()<<"success";
}

void DatabaseAccess::setLoginInfo(const QString &user_name, const QString &password) {
    db.setUserName(user_name);  //设置用户名
    db.setPassword(password);  //设置密码
}
