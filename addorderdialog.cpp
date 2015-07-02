#include "addorderdialog.h"
#include "ui_addorderdialog.h"
#include "tradetypes.h"

addOrderDialog::addOrderDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addOrderDialog),
    db(db)
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
    ui->underlyingInstrTypeComboBox->addItem("SR");
    ui->underlyingInstrTypeComboBox->addItem("0");
    ui->callPutComboBox->addItem("Call");
    ui->callPutComboBox->addItem("Put");
    ui->optionTypeComboBox->addItem("00");
    ui->optionTypeComboBox->addItem("01");
    ui->optionTypeComboBox->addItem("02");
    ui->optionTypeComboBox->addItem("03");
    ui->optionTypeComboBox->addItem("04");
    ui->optionTypeComboBox->addItem("05");
    ui->longShortComboBox->addItem("Long");
    ui->longShortComboBox->addItem("Short");
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
    if (db->writeTransaction(trans))
        QDialog::accept();
}
