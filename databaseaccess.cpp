#include "databaseaccess.h"

#include <QDebug>

using namespace std;

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3309);  //设置端口
    db.setDatabaseName("sugar_opt");  //设置数据库名称
}

void DatabaseAccess::setLoginInfo(const QString &user_name, const QString &password) {
    db.setUserName(user_name);  //设置用户名
    db.setPassword(password);  //设置密码
}

//CREATE TABLE IF NOT EXISTS positions (
//    client_id INT,
//    instr_code VARCHAR(20),
//    average_price DOUBLE,
//    total_amount int,
//    available_amount int,
//    frozen_amount int,
//    long_short ENUM('long', 'short'),
//    underlying_price DOUBLE,
//    occupied_margin DOUBLE,
//    underlying_code VARCHAR(20),
//    knock_out INT,
//    contract_no VARCHAR(20));

