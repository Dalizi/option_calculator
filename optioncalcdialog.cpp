#pragma execution_character_set("utf-8")
#include "optioncalcdialog.h"
#include "ui_optioncalcdialog.h"

#include <QStringList>
#include <QMessageBox>
#include <QDate>

#include "base_function.h"

OptionCalcDialog::OptionCalcDialog(OptionValue *calc_server, DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionCalcDialog),
    calc_server(calc_server),
    db(db)
{
    ui->setupUi(this);
    init();
}

OptionCalcDialog::~OptionCalcDialog()
{
    delete ui;
}

void OptionCalcDialog::init() {
    ui->classCodeComboBox->addItem("SRO");
    ui->classCodeComboBox->addItem("0MO");
    ui->spreadTypeComboBox->addItem("Val");
    ui->optionTypeComboBox->addItem(tr("Call"));
    ui->optionTypeComboBox->addItem(tr("Put"));
    ui->maturityDateEdit->setDate(QDate::currentDate());
    ui->maturityDateEdit->setMinimumDate(QDate::currentDate());
    ui->maturityDateEdit->setCalendarPopup(true);
}

void OptionCalcDialog::on_pricingPushButton_clicked()
{
    PricingParam pp;
    bool ok1, ok2, ok3, ok4, ok5;
    pp.free_rate = ui->freeRateLineEdit->text().toDouble(&ok1);
    pp.option_type = ui->optionTypeComboBox->currentText() == "Call"? 1:2;
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

void OptionCalcDialog::on_getDefaultPushButton_clicked()
{
    auto param_map = db->getParam();
    auto current_class = ui->classCodeComboBox->currentText().toStdString();
    auto param = param_map[current_class];
    ui->volatilityLineEdit->setText(QString::number(param.volatility));
    ui->freeRateLineEdit->setText(QString::number(param.free_rate));
    ui->yieldRateLineEdit->setText(QString::number(param.yield_rate));
}
