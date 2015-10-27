#ifndef DATABASEOPERATION
#define DATABASEOPERATION

#include <QSqlDatabase>
#include <QString>
#include <stdexcept>
#include <string>

class invalid_login_info : public std::runtime_error {
public:
    invalid_login_info() : std::runtime_error("Invalid username/password.") {}
};

class missing_config_file : public std::runtime_error {
public:
    missing_config_file() : std::runtime_error("Missing database configuration file.") {}
};

class DatabaseOperation {
public:
    virtual void add() = 0;
    virtual void modify() = 0;
    virtual void remove() = 0;

private:
    static QSqlDatabase db;
    static QString _username;
    static QString _password;

public:
    static void initDatabase(const std::string &configFile);
    static void setLoginInfo(const QString &username, const QString &password);
    static void login();
};



#endif // DATABASEOPERATION

