#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include "tradetypes.h"

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>

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

signals:

public slots:
};

#endif // DATABASEACCESS_H
