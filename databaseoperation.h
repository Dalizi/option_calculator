#ifndef DATABASEOPERATION
#define DATABASEOPERATION

#include <QSqlDatabase>
#include <QString>
#include <stdexcept>
#include <string>

class InvalidLoginInfo : public std::runtime_error {
public:
    InvalidLoginInfo() : std::runtime_error("Invalid username/password.") {}
};

class MissingConfigFile : public std::runtime_error {
public:
    MissingConfigFile() : std::runtime_error("Missing database configuration file.") {}
};

class DatabaseOperation {
public:
    DatabaseOperation() = default;
    ~DatabaseOperation() = default;

    virtual void Add() = 0;
    virtual void Modify() = 0;
    virtual void Remove() = 0;
    virtual void IsExist() = 0;

private:
    static QSqlDatabase db_;
    static QString username_;
    static QString password_;

public:
    static void InitDatabase(const std::string &configFile);
    static void SetLoginInfo(const QString &username, const QString &password);
    static void Login();
};



#endif // DATABASEOPERATION

