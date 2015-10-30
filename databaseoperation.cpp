#include <fstream>

#include <QSqlError>
#include <QMessageBox>

#include "databaseoperation.h"

using namespace std;

QSqlDatabase DatabaseOperation::db_ = QSqlDatabase::addDatabase("QMYSQL");
QString DatabaseOperation::username_ = "";
QString DatabaseOperation::password_ = "";

void DatabaseOperation::InitDatabase(const string &configFile) {
    ifstream mysql_info(configFile);
    if (!mysql_info.is_open()) {
        throw MissingConfigFile();
    }
    string line;
    if (getline(mysql_info, line)) db_.setHostName(QString::fromStdString(line));
    if (getline(mysql_info, line)) db_.setPort(stod(line));
    if (getline(mysql_info, line)) db_.setDatabaseName(QString::fromStdString(line));
}

void DatabaseOperation::SetLoginInfo(const QString &username, const QString &password) {
    username_ = username;
    password_ = password;
}

void DatabaseOperation::Login() {
    if (!db_.open(username_, password_)) {
        auto nativeEC = db_.lastError().nativeErrorCode();
        if (nativeEC == "1045")
            throw InvalidLoginInfo();
        throw runtime_error(db_.lastError().text().toStdString());
    }
}
