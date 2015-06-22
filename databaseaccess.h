#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QSqlError>

class DatabaseAccess : public QObject
{
    Q_OBJECT
    QSqlDatabase db;
public:
    explicit DatabaseAccess(QObject *parent = 0);
    void setLoginInfo(const QString &user_name, const QString &password);
    bool connectToDatabase() {return db.open();}
    QString lastError() const {return db.lastError().text();}

signals:

public slots:
};

#endif // DATABASEACCESS_H
