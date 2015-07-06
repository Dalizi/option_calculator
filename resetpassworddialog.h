#ifndef RESETPASSWORDDIALOG_H
#define RESETPASSWORDDIALOG_H

#include <QDialog>

#include "databaseaccess.h"

namespace Ui {
class ResetPasswordDialog;
}

class ResetPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetPasswordDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~ResetPasswordDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ResetPasswordDialog *ui;
    DatabaseAccess *db;
};

#endif // RESETPASSWORDDIALOG_H
