#include "optioncalcdialog.h"
#include "ui_optioncalcdialog.h"

#include <QStringList>
#include <QMessageBox>
#include <QDate>

#include <base_function.h>

OptionCalcDialog::OptionCalcDialog(OptionValue *calc_server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionCalcDialog),
    calc_server(calc_server)
{
    ui->setupUi(this);
    init();
}

OptionCalcDialog::~OptionCalcDialog()
{
    delete ui;
}

void OptionCalcDialog::init() {
    ui->classCodeComboBox->addItems(QStringList({"SRO", "0MO"}));
    ui->spreadTypeComboBox->addItems(QStringList({"Val"}));
    ui->optionTypeComboBox->addItems(QStringList({tr("Call"), tr("Put")}));
    ui->maturityDateEdit->setDate(QDate::currentDate());
    ui->maturityDateEdit->setMinimumDate(QDate::currentDate());
    ui->maturityDateEdit->setCalendarPopup(true);
}

void OptionCalcDialog::on_pricingPushButton_clicked()
{
    PricingParam pp;
    bool ok1, ok2, ok3, ok4, ok5;
    pp.multiplier = ui->multiplierLineEdit->text().toInt(&ok1);
    pp.option_type = ui->optionTypeComboBox->currentText() == "Call"? 0:1;
    pp.yield_rate = ui->yieldRateLineEdit->text().toDouble(&ok2);
    pp.volatility = ui->volatilityLineEdit->text().toDouble(&ok3);
    auto maturity_date = ui->maturityDateEdit->date().toString("yyyy-MM-dd").toStdString();
    pp.time_to_maturity = calc_server->Maturity_Remain(maturity_date);
    pp.spot_price = ui->spotPriceLineEdit->text().toDouble(&ok4);
    pp.strike_price = ui->strikePriceLineEdit->text().toDouble(&ok5);

    if (!(ok1 && ok2 && ok3 && ok4 && ok5)){
        QMessageBox::warning(this, "Pricing Failed", "Missing or invalid input in required fields.");
        return;
    }
    auto price = bls_vanila_option(pp);
    ui->priceLineEdit->setText(QString::number(price));
}
