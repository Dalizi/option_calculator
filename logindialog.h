#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "databaseaccess.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~LoginDialog();

public slots:
    virtual void accept();

private:
    Ui::LoginDialog *ui;
    DatabaseAccess *db;
};

#endif // LOGINDIALOG_H
