#include "transactionviewdialog.h"
#include "ui_transactionviewdialog.h"

#include <QSqlTableModel>

transactionViewDialog::transactionViewDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transactionViewDialog),
    db(db)
{
    ui->setupUi(this);
    init();
}

transactionViewDialog::~transactionViewDialog()
{
    delete ui;
}

void transactionViewDialog::init() {
    auto model = new QSqlTableModel(0, db->getDatabase());
    model->setTable("transactions");
    model->select();
    ui->transactionTableView->setModel(model);
}
