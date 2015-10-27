#include <fstream>

#include <QSqlError>
#include <QMessageBox>

#include "databaseoperation.h"

using namespace std;

QSqlDatabase DatabaseOperation::db = QSqlDatabase::addDatabase("QMYSQL");
QString DatabaseOperation::_username = "";
QString DatabaseOperation::_password = "";

void DatabaseOperation::initDatabase(const string &configFile) {
    ifstream mysql_info(configFile);
    if (!mysql_info.is_open()) {
        throw missing_config_file();
    }
    string line;
    if (getline(mysql_info, line)) db.setHostName(QString::fromStdString(line));
    if (getline(mysql_info, line)) db.setPort(stod(line));
    if (getline(mysql_info, line)) db.setDatabaseName(QString::fromStdString(line));
}

void DatabaseOperation::setLoginInfo(const QString &username, const QString &password) {
    _username = username;
    _password = password;
}

void DatabaseOperation::login() {
    if (!db.open(_username, _password)) {
        auto nativeEC = db.lastError().nativeErrorCode();
        if (nativeEC == "1045")
            throw invalid_login_info();
        throw runtime_error(db.lastError().text().toStdString());
    }
}
