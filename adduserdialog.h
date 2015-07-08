#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

#include "databaseaccess.h"

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~AddUserDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddUserDialog *ui;
    DatabaseAccess *db;
};

#endif // ADDUSERDIALOG_H
