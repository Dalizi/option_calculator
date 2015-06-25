#include "databaseaccess.h"


#include <QDebug>
#include <QMessageBox>

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

void DatabaseAccess::writeTransaction(TransactionType trans) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO positions (client_id, instr_code, price, amount, long_short, open_offset, underlying_price, underlying_code, close_pnl)"
                  " VALUES(:client_id, :instr_code, :price, :amount, :long_short, :open_offset, :underlying_price, :underlying_code, :close_pnl);");
    query.bindValue(":client_id", trans.client_id);
    query.bindValue(":instr_code", trans.instr_code);
    query.bindValue(":price", trans.price);
    query.bindValue(":amount", trans.amount);
    query.bindValue(":long_short", trans.long_short == LONG_ORDER?"long":"short");
    query.bindValue(":open_offset", trans.open_offset == OPEN?"open":"offset");
    query.bindValue(":underlying_price", trans.underlying_price);
    query.bindValue(":underlying_code", trans.underlying_code);
    query.bindValue(":close_pnl", trans.close_pnl);
    if (!query.exec()) {
        QMessageBox::warning(0, "写入成交记录失败", query.lastError().text());
    }
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

