#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include "tradetypes.h"
#include "valuation_class.h"

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>

#include <vector>
#include <string>

class DatabaseAccess : public QObject
{
    Q_OBJECT
    QSqlDatabase db;

    QString genContractNum(int client_id);
public:
    enum UserPrivilege {
        ADMIN = 0,
        BROKER = 1,
        QUANT = 2
    };

    explicit DatabaseAccess(QObject *parent = 0);
    void setLoginInfo(const QString &user_name, const QString &password);
    bool connectToDatabase() {return db.open();}
    QString lastError() const {return db.lastError().text();}
    QSqlDatabase& getDatabase() {return db;}
    bool writeTransaction(TransactionType trans_type);
    void updatePosition(PositionType &pt, const TransactionType &tt);

    std::vector<PositionType> getAllPosition(const QString &instr_type);
    std::map<std::string, PricingParam> getParam();
    bool setParam(const QString &code, const PricingParam &param);
    QStringList getAllClassCode();
    bool setPassword(const QString new_passwd, const QString old_password);
    bool addUser(const QString user_name, const QString init_password, UserPrivilege priv);

private:
    void loadConfig(const std::string &configFile);
    QString username;
    QString password;

signals:
    void transactionWritten(TransactionType trans);

public slots:
    bool writePosition(const TransactionType &trans);
};

#endif // DATABASEACCESS_H
