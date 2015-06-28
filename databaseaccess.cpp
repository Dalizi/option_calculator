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
    connect(this, SIGNAL(transactionWritten(TransactionType)), this, SLOT(writePosition(TransactionType)));
}

void DatabaseAccess::setLoginInfo(const QString &user_name, const QString &password) {
    db.setUserName(user_name);  //设置用户名
    db.setPassword(password);  //设置密码
}

bool DatabaseAccess::writeTransaction(TransactionType trans) {
    QSqlQuery query(db);
    trans.contract_no = genContractNum();
    query.prepare("INSERT INTO transactions (client_id, instr_code, price, amount, long_short, open_offset, underlying_price, underlying_code, close_pnl, knockout_price, contract_no)"
                  " VALUES(:client_id, :instr_code, :price, :amount, :long_short, :open_offset, :underlying_price, :underlying_code, :close_pnl, :knockout_price, :contract_no);");
    query.bindValue(":client_id", trans.client_id);
    query.bindValue(":instr_code", trans.instr_code);
    query.bindValue(":price", trans.price);
    query.bindValue(":amount", trans.amount);
    query.bindValue(":long_short", trans.long_short == LONG_ORDER?"long":"short");
    query.bindValue(":open_offset", trans.open_offset == OPEN?"open":"offset");
    query.bindValue(":underlying_price", trans.underlying_price);
    query.bindValue(":underlying_code", trans.underlying_code);
    query.bindValue(":close_pnl", trans.close_pnl);
    query.bindValue(":knockout_price", trans.knockout_price);
    query.bindValue(":contract_no", trans.contract_no);
    if (!query.exec()) {
        QMessageBox::warning(0, "写入成交记录失败", query.lastError().text());
        return false;
    }
    //emit transactionWritten(trans);
    if (!writePosition(trans)) {
        return false;
    }
    return true;
}

bool DatabaseAccess::writePosition(const TransactionType &trans) {
//    QSqlQuery query(db);
//    query.prepare("SELECT * FROM position WHERE client_id=:client_id, instr_code=:instr_code, long_short=:long_short"); //读取成交对应的持仓信息
//    query.bindValue(":client_id", trans.client_id);
//    query.bindValue(":instr_code", trans.instr_code);
//    query.bindValue(":long_short", trans.long_short == LONG_ORDER?"long":"short");
//    if (!query.exec())
//        QMessageBox::warning(0, "读取持仓信息失败", query.lastError().text());
//    if (!query.next()) {                                                                                                //如果不存在则插入对应持仓信息
//        query.prepare("INSERT INTO positions (client_id, instr_code, average_price, total_amount, available_amount, frozen_amount, long_short, underlying_price, underlying_code)"
//                      " VALUES(:client_id, :instr_code, :average_price, :total_amount, :available_amount, :frozen_amount, :long_short, :underlying_price, :underlying_code);");
//        query.bindValue(":client_id", trans.client_id);
//        query.bindValue(":instr_code", trans.instr_code);
//        query.bindValue(":long_short", trans.long_short == LONG_ORDER?"long":"short");
//        query.bindValue(":average_price", trans.price);
//        query.bindValue(":total_amount", trans.amount);
//        query.bindValue(":available_amount", trans.amount);
//        query.bindValue(":frozen_amount", 0);
//        query.bindValue(":underlying_code", trans.underlying_code);
//        query.bindValue(":underlying_price", trans.underlying_price);
//        if (!query.exec())
//            QMessageBox::warning(0, "添加持仓信息失败", query.lastError().text());
//    } else {                                                                                                            //如果存在则修改并更新持仓信息
//        PositionType position;
//        position.client_id = query.value("client_id").toInt();
//        position.instr_code = query.value("instr_code").toString();
//        position.average_price = query.value("average_price").toDouble();
//        position.total_amount = query.value("total_amount").toInt();
//        position.available_amount = query.value("available_amount").toInt();
//        position.frozen_amount = query.value("frozen_amount").toInt();
//        position.long_short = query.value("long_short").toString()=="long"?LONG_ORDER:SHORT_ORDER;
//        position.occupied_margin = query.value("occupied_margin").toDouble();
//        position.underlying_code = query.value("underlying_code ").toString();
//        position.underlying_price = query.value("underlying_price").toDouble();
//        updatePosition(position, trans);

//        query.prepare("INSERT INTO positions (client_id, instr_code, average_price, total_amount, available_amount, frozen_amount, long_short, underlying_price, underlying_code)"
//                      " VALUES(:client_id, :instr_code, :average_price, :total_amount, :available_amount, :frozen_amount, :long_short, :underlying_price, :underlying_code);");
//        query.bindValue(":client_id", position.client_id);
//        query.bindValue(":instr_code", position.instr_code);
//        query.bindValue(":long_short", position.long_short == LONG_ORDER?"long":"short");
//        query.bindValue(":average_price", position.average_price);
//        query.bindValue(":total_amount", position.total_amount);
//        query.bindValue(":available_amount", position.available_amount);
//        query.bindValue(":frozen_amount", position.total_amount-position.available_amount);
//        query.bindValue(":underlying_code", position.underlying_code);
//        query.bindValue(":underlying_price", position.underlying_price);
//        if (!query.exec())
//            QMessageBox::warning(0, "更新持仓信息失败", query.lastError().text());
//    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO positions (client_id, instr_code, average_price, total_amount, available_amount, frozen_amount, long_short, underlying_price, underlying_code, knockout_price, contract_no)"
                  " VALUES(:client_id, :instr_code, :average_price, :total_amount, :available_amount, :frozen_amount, :long_short, :underlying_price, :underlying_code, :knockout_price, :contract_no);");
    query.bindValue(":client_id", trans.client_id);
    query.bindValue(":instr_code", trans.instr_code);
    query.bindValue(":long_short", trans.long_short == LONG_ORDER?"long":"short");
    query.bindValue(":average_price", trans.price);
    query.bindValue(":total_amount", trans.amount);
    query.bindValue(":available_amount", trans.amount);
    query.bindValue(":frozen_amount", 0);
    query.bindValue(":underlying_code", trans.underlying_code);
    query.bindValue(":underlying_price", trans.underlying_price);
    query.bindValue(":knockout_price", trans.knockout_price);
    query.bindValue(":contract_no", trans.contract_no);
    if (!query.exec()) {
        QMessageBox::warning(0, "添加持仓信息失败", query.lastError().text());
        return false;
    }
    return true;
}

void DatabaseAccess::updatePosition(PositionType &pt, const TransactionType &tt) {
//    QSqlQuery query(db);
//    int adjust_param = tt.open_offset == OpenOffsetType::OPEN ? 1 : -1;
//    int total_amount = pt.total_amount + tt.amount * adjust_param;
//    double average_price, underlying_price;
//    if (total_amount != 0) {
//        if (tt.open_offset == OPEN) {
//            average_price = (pt.average_price * pt.total_amount + tt.price * tt.amount)/ (pt.total_amount + tt.amount);
//            underlying_price = (pt.underlying_price * pt.total_amount + tt.underlying_price * tt.amount) / (pt.total_amount + tt.amount) ;
//        } else {
//            average_price = pt.average_price;
//            underlying_price = pt.underlying_price;
//        }

//        query.prepare("UPDATE position SET total_amount=?, available_amount=?,average_price=?, underlying_price=?, occupied_margin=?"
//                      "WHERE instr_code=? AND client_id=? AND long_short=?");
//        query.addBindValue(total_amount);
//        query.addBindValue(total_amount);
//        query.addBindValue(average_price);
//        query.addBindValue(underlying_price);
//        query.addBindValue(pt.occupied_margin);
//        query.addBindValue(pt.instr_code);
//        query.addBindValue(pt.client_id);
//        query.addBindValue(pt.long_short==LONG_ORDER?"long":"short");
//    } else {
//        query.prepare("DELETE FROM position WHERE instr_code=? AND client_id=? AND long_short=?");
//        query.addBindValue(pt.instr_code);
//        query.addBindValue(pt.client_id);
//        query.addBindValue(pt.long_short==LONG_ORDER?"long":"short");
//    }

//    if (!query.exec())
//        qDebug() << "Update position FAILED..." << " QUERY is: " << query.lastQuery() << " ERROR is: " << query.lastError();


}

//CREATE TABLE IF NOT EXISTS positions (
//    client_id INT NOT NULL,
//    instr_code VARCHAR(40) NOT NULL,
//    average_price DOUBLE NOT NULL,
//    total_amount int NOT NULL,
//    available_amount int NOT NULL,
//    frozen_amount int NOT NULL,
//    long_short ENUM('long', 'short') NOT NULL,
//    underlying_price DOUBLE NOT NULL,
//    occupied_margin DOUBLE NOT NULL,
//    underlying_code VARCHAR(20) NOT NULL,
//    knockout_price DOUBLE NOT NULL,
//    contract_no VARCHAR(20) NOT NULL);

//CREATE TABLE IF NOT EXISTS transactions (
//        time TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
//        client_id INT NOT NULL,
//        instr_code VARCHAR(40) NOT NULL,
//        price DOUBLE NOT NULL,
//        amount INT NOT NULL,
//        long_short ENUM('long', 'short') NOT NULL,
//        open_offset ENUM('open', 'offset') NOT NULL,
//        underlying_price DOUBLE NOT NULL,
//        underlying_code VARCHAR(20) NOT NULL,
//        close_pnl DOUBLE NOT NULL,
//        knockout_price DOUBLE NOT NULL,
//        contract_no VARCHAR(20) NOT NULL);

vector<PositionType> DatabaseAccess::getAllPosition() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM positions");
    if (!query.exec())
        QMessageBox::warning(0, "读取所有持仓失败", query.lastError().text());
    vector<PositionType> ret;
    while (query.next()) {
        PositionType position;
        position.client_id = query.value("client_id").toInt();
        position.instr_code = query.value("instr_code").toString();
        position.average_price = query.value("average_price").toDouble();
        position.total_amount = query.value("total_amount").toInt();
        position.available_amount = query.value("available_amount").toInt();
        position.frozen_amount = query.value("frozen_amount").toInt();
        position.long_short = query.value("long_short").toString()=="long"?LONG_ORDER:SHORT_ORDER;
        position.occupied_margin = query.value("occupied_margin").toDouble();
        position.underlying_code = query.value("underlying_code ").toString();
        position.underlying_price = query.value("underlying_price").toDouble();
        position.knockout_price= query.value("knockout_price").toDouble();
        ret.push_back(position);
    }
    return ret;

}

void DatabaseAccess::test() {
    QSqlQuery query(db);
    query.prepare("SHOW GRANTS FOR current_user;");
    if (!query.exec())
        qDebug() << query.lastError();
    qDebug() <<query.value(0).toString();
}

QString DatabaseAccess::genContractNum() {
    return "test";
}
