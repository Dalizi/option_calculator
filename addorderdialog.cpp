#include "addorderdialog.h"
#include "ui_addorderdialog.h"
#include "tradetypes.h"

addOrderDialog::addOrderDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addOrderDialog),
    db(db)
{
    ui->setupUi(this);
    init();
}

addOrderDialog::~addOrderDialog()
{
    delete ui;
}

void addOrderDialog::init() {
    ui->underlyingInstrTypeComboBox->addItems(QStringList({"SR", "IF"}));
    ui->callPutComboBox->addItems(QStringList({"Call", "Put"}));
    ui->optionTypeComboBox->addItems(QStringList({"00", "01", "02", "03", "04", "05"}));
    ui->longShortComboBox->addItems(QStringList({"Long", "Short"}));
}

void addOrderDialog::accept() {
    TransactionType trans;
    QString instr_code = "OTC-";
    instr_code += ui->underlyingInstrTypeComboBox->currentText();
    instr_code += 'X';
    instr_code += ui->callPutComboBox->currentText() == "Call"?'C':'P';
    instr_code += ui->optionTypeComboBox->currentText();
    instr_code += '-';
    instr_code += ui->strikePriceLineEdit->text();
    trans.instr_code = instr_code;
    trans.client_id = ui->clientIDLineEdit->text().toInt();
    trans.price = ui->priceLineEdit->text().toDouble();
    trans.amount = ui->amountLineEdit->text().toInt();
    trans.long_short = ui->longShortComboBox->currentText() == "Long"?LONG_ORDER:SHORT_ORDER;
    trans.underlying_code = ui->underlyingCodeLineEdit->text();
    db->writeTransaction(trans);

}
