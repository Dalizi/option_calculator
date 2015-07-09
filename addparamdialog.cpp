#include "addparamdialog.h"
#include "ui_addparamdialog.h"
#include "base_function.h"

#include <QMessageBox>

AddParamDialog::AddParamDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddParamDialog),
    db(db)
{
    ui->setupUi(this);
    ui->spreadTypeComboBox->addItem("Vol");
}

AddParamDialog::~AddParamDialog()
{
    delete ui;
}

void AddParamDialog::on_buttonBox_accepted()
{
    PricingParam pp;
    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7;
    pp.free_rate = ui->freeRateLineEdit->text().toDouble(&ok1);
    pp.multiplier = ui->multiplierLineEdit->text().toInt(&ok2);
    pp.volatility = ui->volatilityLineEdit->text().toDouble(&ok3);
    pp.yield_rate = ui->yieldRateLineEdit->text().toDouble(&ok4);
    pp.other_param["basis_delta_spread"] = ui->basisDeltaSpreadLineEdit->text().toDouble(&ok5);
    pp.other_param["basis_vol_spread"] = ui->basisVolSpreadLineEdit->text().toDouble(&ok6);
    pp.other_param["basis_price_spread"] = ui->basisPriceSpreadLineEdit->text().toDouble(&ok7);
    if (!(ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7))
        QMessageBox::warning(this, "Invalid input", "Invaid input.");
    if (!db->setParam(ui->classCodeLineEdit->text(), pp))
        QMessageBox::warning(this, "Error", "Adding param failed.");
}
