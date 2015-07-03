#ifndef TRANSACTIONVIEWDIALOG_H
#define TRANSACTIONVIEWDIALOG_H
#include "databaseaccess.h"

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class transactionViewDialog;
}

class transactionViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit transactionViewDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~transactionViewDialog();

private slots:
    void on_queryPushButton_clicked();

private:
    Ui::transactionViewDialog *ui;
    DatabaseAccess *db;
    QSqlTableModel *model;

    void init();
};

#endif // TRANSACTIONVIEWDIALOG_H
