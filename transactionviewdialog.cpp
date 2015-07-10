#include "transactionviewdialog.h"
#include "ui_transactionviewdialog.h"

#include <QSqlTableModel>
#include <QMessageBox>

transactionViewDialog::transactionViewDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transactionViewDialog),
    db(db),
    model(new QSqlTableModel(0, db->getDatabase()))
{
    ui->setupUi(this);
    init();
}

transactionViewDialog::~transactionViewDialog()
{
    delete ui;
}

void transactionViewDialog::init() {
    model->setTable("transactions");
    model->select();
    ui->transactionTableView->setModel(model);
}

void transactionViewDialog::on_queryPushButton_clicked()
{
    bool ok;
    auto client_id = ui->chooseClientLineEdit->text().toInt(&ok);
    if (ok) {
        QString filterText = "client_id=%1";
        model->setFilter(filterText.arg(client_id));
    }
    model->select();
}
