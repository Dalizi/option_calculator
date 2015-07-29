#ifndef TRANSACTIONVIEWDIALOG_H
#define TRANSACTIONVIEWDIALOG_H
#include "databaseaccess.h"
#include "tradetypes.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

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

    void on_acceptPushButton_clicked();
    void on_declinePushButton_clicked();

    void on_chooseStatusComboBox_currentIndexChanged(const QString &arg1);

    void on_resummitPushButton_clicked();

private:
    Ui::transactionViewDialog *ui;
    DatabaseAccess *db;
    QSqlTableModel *model;
    QString client_filter;
    QString status_filter;

    void init();
    TransactionType extractTransaction(const QSqlRecord &record);
};

#endif // TRANSACTIONVIEWDIALOG_H
