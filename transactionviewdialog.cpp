#include "transactionviewdialog.h"
#include "ui_transactionviewdialog.h"

#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>

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
    ui->chooseStatusComboBox->addItem(tr("all"));
    ui->chooseStatusComboBox->addItem(tr("accepted"));
    ui->chooseStatusComboBox->addItem(tr("declined"));
    ui->chooseStatusComboBox->addItem(tr("pending"));
}

void transactionViewDialog::on_queryPushButton_clicked()
{
    bool ok;
    auto client_id = ui->chooseClientLineEdit->text().toInt(&ok);
    if (ok) {
        client_filter = QString("client_id=%1").arg(client_id);
    } else
        client_filter= "";
    if (status_filter != "")
        model->setFilter(client_filter + " and " + status_filter);
    else
        model->setFilter(client_filter);
    qDebug() <<model->filter();
    model->select();
}

void transactionViewDialog::on_acceptPushButton_clicked()
{
    auto selection = ui->transactionTableView->selectionModel()->selectedRows();
    for (auto index:selection) {
        auto record = model->record(index.row());
        record.setValue("status", "accepted");
        model->setRecord(index.row(), record);
    }
}

void transactionViewDialog::on_declinePushButton_clicked()
{
    auto selection = ui->transactionTableView->selectionModel()->selectedRows();
    for (auto index:selection) {
        auto record = model->record(index.row());
        record.setValue("status", "declined");
        model->setRecord(index.row(), record);
    }
}

void transactionViewDialog::on_chooseStatusComboBox_currentIndexChanged(const QString &status)
{
    if (status == "all")
        status_filter = "";
    else
        status_filter = QString("status='%1'").arg(status);
    if (client_filter != "")
        model->setFilter(client_filter + " AND " + status_filter);
    else
        model->setFilter(status_filter);
    qDebug() <<model->filter();
    model->select();
}

void transactionViewDialog::on_resummitPushButton_clicked()
{
    auto selection = ui->transactionTableView->selectionModel()->selectedRows();
    for (auto index:selection) {
        auto record = model->record(index.row());
        record.setValue("status", "pending");
        model->setRecord(index.row(), record);
    }
}

TransactionType transactionViewDialog::extractTransaction(const QSqlRecord &record) {
    TransactionType trans;
    trans.amount = record.value("amount").toInt();
    trans.client_id = record.value("client_id").toInt();
    trans.close_pnl = record.value("close_pnl").toDouble();
    trans.contract_no = record.value("contract_no").toString();
    trans.implied_vol = record.value("implied_vol").toDouble();
    trans.instr_code = record.value("instr_code").toString();
    trans.knockout_price = record.value("knockout_price").toDouble();
    trans.long_short = record.value("long_short").toString() == "long"?LONG_ORDER:SHORT_ORDER;
    trans.open_offset = record.value("open_offset").toString() == "open"?OPEN:OFFSET;
    trans.price = record.value("price").toDouble();
    trans.time = record.value("time").toDateTime();
    trans.transaction_id = record.value("transaction_id").toString();
    trans.underlying_code = record.value("underlying_code").toString();
    trans.underlying_price = record.value("underlying_price").toDouble();
    return trans;
}
