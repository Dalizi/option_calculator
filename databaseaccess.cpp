#pragma execution_character_set("utf-8")
#include "databaseaccess.h"

#include <QDebug>
#include <QMessageBox>

#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    loadConfig("mysql_config.ini");
    connect(this, SIGNAL(transactionWritten(TransactionType)), this, SLOT(writePosition(TransactionType)));
}

void DatabaseAccess::setLoginInfo(const QString &user_name, const QString &password) {
    db.setUserName(user_name);  //设置用户名
    db.setPassword(password);  //设置密码
    username=user_name;
    this->password = password;
}

void DatabaseAccess::loadConfig(const string &configFile) {
    ifstream mysql_info(configFile);
    if (!mysql_info.is_open()) {
        QMessageBox::warning(0, "Warning", "读取sql数据库连接配置文件失败。");
        exit(1);
    }
    string line;
    if (getline(mysql_info, line)) db.setHostName(QString::fromStdString(line));
    if (getline(mysql_info, line)) db.setPort(stod(line));
    if (getline(mysql_info, line)) db.setDatabaseName(QString::fromStdString(line));
}

bool DatabaseAccess::writeTransaction(TransactionType trans) {
    QSqlQuery query(db);
    trans.contract_no = genContractNum(trans.client_id);
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




vector<PositionType> DatabaseAccess::getAllPosition(const QString &instr_type) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM positions WHERE instr_code LIKE 'OTC-"+instr_type+"%'");
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
        position.underlying_code = query.value("underlying_code").toString();
        position.underlying_price = query.value("underlying_price").toDouble();
        position.knockout_price= query.value("knockout_price").toDouble();
        ret.push_back(position);
    }
    return ret;

}

QString DatabaseAccess::genContractNum(int client_id) {
    QSqlQuery query(db);
    query.prepare("SELECT count(*) from transactions WHERE client_id=?");
    query.addBindValue(client_id);
    if (!query.exec()) {
        QMessageBox::warning(0, tr("Getting transaction count failed"), query.lastError().text());
    }
    string count;
    if (query.next())
        count = query.value(0).toString().toStdString();
    stringstream ss;
    ss << setw(6) <<setfill('0') <<client_id;
    ss << setw(10) <<setfill('0') <<count;
    return QString::fromStdString(ss.str());
}

map<string, PricingParam> DatabaseAccess::getParam() {
    map<string, PricingParam> ret;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM param;");
    if (!query.exec())
        QMessageBox::warning(0, tr("Reading parameters failed"), query.lastError().text());
    while (query.next()) {
        PricingParam pp;
        pp.free_rate = query.value("free_rate").toDouble();
        pp.multiplier = query.value("multiplier").toInt();
        pp.yield_rate = query.value("yield_rate").toDouble();
        pp.volatility = query.value("volatility").toDouble();
        for (auto s:vector<QString>{"basis_delta_spread", "basis_price_spread", "basis_vol_spread", "spread_type"})
            pp.other_param[s.toStdString()] = query.value(s).toString().toStdString();

//        string basis_delta_spread = query.value("basis_delta_spread").toString().toStdString();
//        string basis_price_spread = query.value("basis_price_spread").toString().toStdString();
//        string basis_vol_spread = query.value("basis_vol_spread").toString().toStdString();
//        string spread_type = query.value("spread_type").toString().toStdString();
//        pp.other_param["basis_delta_spread"]
        ret[query.value("class_code").toString().toStdString()] = pp;
    }
    return ret;
}

bool DatabaseAccess::setParam(const QString &code, const PricingParam &param) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO param (class_code, basis_delta_spread, basis_price_spread, basis_vol_spread, free_rate, multiplier, spread_type, volatility, yield_rate )"
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(code);
    query.addBindValue(QString::fromStdString(param.other_param.at("basis_delta_spread")));
    query.addBindValue(QString::fromStdString(param.other_param.at("basis_price_spread")));
    query.addBindValue(QString::fromStdString(param.other_param.at("basis_vol_spread")));
    query.addBindValue(param.free_rate);
    query.addBindValue(param.multiplier);
    query.addBindValue("Vol");
    query.addBindValue(param.volatility);
    query.addBindValue(param.yield_rate);
    if (!query.exec()) {
        QMessageBox::warning(0, tr("Setting parameters failed"), query.lastError().text());
        return false;
    }
    return true;
}

QStringList DatabaseAccess::getAllClassCode() {
    QStringList ret;
    QSqlQuery query(db);
    query.prepare("SELECT class_code FROM param;");
    if (!query.exec())
        QMessageBox::warning(0, tr("Reading class code failed"), query.lastError().text());
    while (query.next())
        ret.push_back(query.value(0).toString());
    return ret;
}

bool DatabaseAccess::setPassword(const QString new_passwd, const QString old_password) {
    if (old_password!= password) {
        QMessageBox::warning(0, tr("Error"), tr("Incorrect old password."));
        return false;
    }
    QSqlQuery query(db);
    query.prepare("SET PASSWORD = PASSWORD('" + new_passwd + "')");
    if (!query.exec()) {
        QMessageBox::warning(0, tr("Error"), tr("Resetting password failed."));
        return false;
    }
    return true;
}

bool DatabaseAccess::addUser(const QString user_name, const QString init_password, UserPrivilege priv) {
    QString ip_addr;
    if (priv == ADMIN)
        ip_addr = "localhost";
    else
        ip_addr = "%";
    QString query_text = "CREATE USER '%1'@'%2' IDENTIFIED BY'%3'";
    QSqlQuery query(db);
    query.prepare(query_text.arg(user_name).arg(ip_addr).arg(init_password));
    if (!query.exec()) {
        QMessageBox::warning(0, tr("Error"), query.lastError().text());
        return false;
    }

    QString priv_str = "GRANT %1 ON %2 TO '%3'@'%4'";
    auto err_func = [user_name, ip_addr](QSqlQuery &query) {
        QMessageBox::warning(0, tr("Error"), query.lastError().text());
        QString drop_user_str = "DROP USER '%1'@'%2'";
        Q_ASSERT(query.exec(drop_user_str.arg(user_name).arg(ip_addr)));
    };
    if (priv == ADMIN) {
        query.prepare(priv_str.arg("ALL").arg("sugar_opt.*").arg(user_name).arg("localhost"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
    } else if (priv == BROKER) {
        query.prepare(priv_str.arg("INSERT, SELECT, DELETE, UPDATE").arg("sugar_opt.positions").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
        query.prepare(priv_str.arg("INSERT, SELECT, DELETE, UPDATE").arg("sugar_opt.transactions").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
        query.prepare(priv_str.arg("SELECT").arg("sugar_opt.param").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
    } else if (priv == QUANT) {
        query.prepare(priv_str.arg("INSERT, SELECT, DELETE, UPDATE").arg("sugar_opt.param").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
        query.prepare(priv_str.arg("SELECT").arg("sugar_opt.transactions").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
        query.prepare(priv_str.arg("SELECT").arg("sugar_opt.positions").arg(user_name).arg("%"));
        if (!query.exec()) {
            err_func(query);
            return false;
        }
    } else {
        QMessageBox::warning(0, tr("Error"), tr("Unknown user type."));
    }
    if (!query.exec(QString("INSERT INTO user_info (user_name, user_priv) VALUES('%1', '%2');").arg(user_name).arg(priv == ADMIN?"Admin":(priv == BROKER?"Broker":"Quant"))))
        err_func(query);
    return true;

}

//#Sql command
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
//CREATE TABLE IF NOT EXISTS param (
//        class_code VARCHAR(10) NOT NULL PRIMARY KEY,
//        basis_delta_spread DOUBLE NOT NULL,
//        basis_price_spread DOUBLE NOT NULL,
//        basis_vol_spread DOUBLE NOT NULL,
//        free_rate DOUBLE NOT NULL,
//        multiplier INT NOT NULL,
//        spread_type VARCHAR(5) NOT NULL,
//        volatility DOUBLE NOT NULL,
//        yield_rate DOUBLE NOT NULL);

//CREATE TABLE IF NOT EXISTS user_info (
//        user_name VARCHAR(20) NOT NULL PRIMARY KEY,
//        user_priv ENUM('Admin', 'Broker', 'Quant'));

//INSERT INTO param (class_code, basis_delta_spread, basis_price_spread, basis_vol_spread, free_rate, multiplier, spread_type, volatility, yield_rate)
//VALUES ('SRO', 0, 0, 0.012, 0.05, 100, 'Vol', 0.4, 0);

//INSERT INTO param (class_code, basis_delta_spread, basis_price_spread, basis_vol_spread, free_rate, multiplier, spread_type, volatility, yield_rate)
//VALUES ('0MO', 0, 0, 0.012, 0.05, 100, 'Vol', 0.4, 0);

