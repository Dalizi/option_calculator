#ifndef TRANSACTIONVIEWDIALOG_H
#define TRANSACTIONVIEWDIALOG_H
#include "databaseaccess.h"

#include <QDialog>

namespace Ui {
class transactionViewDialog;
}

class transactionViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit transactionViewDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~transactionViewDialog();

private:
    Ui::transactionViewDialog *ui;
    DatabaseAccess *db;

    void init();
};

#endif // TRANSACTIONVIEWDIALOG_H
