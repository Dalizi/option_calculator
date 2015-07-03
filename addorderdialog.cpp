#include "addorderdialog.h"
#include "ui_addorderdialog.h"
#include "tradetypes.h"

#include <QMessageBox>

addOrderDialog::addOrderDialog(DatabaseAccess *db, CAccessRedis *redis, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addOrderDialog),
    db(db),
    redis(redis)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), parent, SLOT(onTransactionWritten()));
    init();
}

addOrderDialog::~addOrderDialog()
{
    delete ui;
}

void addOrderDialog::init() {
    ui->underlyingInstrTypeComboBox->addItems(QStringList({"SR", "0M"}));
    ui->callPutComboBox->addItems(QStringList({"Call", "Put"}));
    ui->optionTypeComboBox->addItems(QStringList({"00", "01", "02", "03", "04", "05"}));
    ui->longShortComboBox->addItems(QStringList({"Long", "Short"}));
    ui->maturityDateEdit->setDate(QDate::currentDate());
    ui->maturityDateEdit->setMinimumDate(QDate::currentDate());
    ui->maturityDateEdit->setCalendarPopup(true);
}

void addOrderDialog::accept() {
    TransactionType trans;
    QString instr_code = "OTC-";
    instr_code += ui->underlyingInstrTypeComboBox->currentText();
    instr_code += 'O';
    instr_code += ui->callPutComboBox->currentText() == "Call"?'C':'P';
    instr_code += ui->optionTypeComboBox->currentText();
    instr_code += '-';
    instr_code += ui->maturityDateEdit->date().toString("yyyy-MM-dd");
    instr_code += '-';
    instr_code += ui->strikePriceLineEdit->text();
    trans.instr_code = instr_code;
    trans.client_id = ui->clientIDLineEdit->text().toInt();
    trans.price = ui->priceLineEdit->text().toDouble();
    trans.amount = ui->amountLineEdit->text().toInt();
    trans.long_short = ui->longShortComboBox->currentText() == "Long"?LONG_ORDER:SHORT_ORDER;
    trans.underlying_code = ui->underlyingCodeLineEdit->text();
    trans.underlying_price = ui->underlyingPriceLineEdit->text().toDouble();
    trans.knockout_price = ui->kickOutPriceLineEdit->text().toDouble();
    bool isUnderlyingCodeExists;
    redis->Exists(trans.underlying_code.toStdString(), isUnderlyingCodeExists);
    if (!isUnderlyingCodeExists) {
        QMessageBox::warning(this, "Error", "Underlying code doesn't exist.");
        return;
    }
    if (db->writeTransaction(trans) && isUnderlyingCodeExists)
        QDialog::accept();
}
