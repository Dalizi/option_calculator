#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include "tradetypes.h"

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>

#include <vector>

class DatabaseAccess : public QObject
{
    Q_OBJECT
    QSqlDatabase db;
public:
    explicit DatabaseAccess(QObject *parent = 0);
    void setLoginInfo(const QString &user_name, const QString &password);
    bool connectToDatabase() {return db.open();}
    QString lastError() const {return db.lastError().text();}
    QSqlDatabase& getDatabase() {return db;}
    void writeTransaction(TransactionType trans_type);
    void updatePosition(PositionType &pt, const TransactionType &tt);

    std::vector<PositionType> getAllPosition();

signals:
    void transactionWritten(TransactionType trans);

public slots:
    void writePosition(const TransactionType &trans);
};

#endif // DATABASEACCESS_H
