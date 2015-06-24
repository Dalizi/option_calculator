#include "addorderdialog.h"
#include "ui_addorderdialog.h"

addOrderDialog::addOrderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addOrderDialog)
{
    ui->setupUi(this);
}

addOrderDialog::~addOrderDialog()
{
    delete ui;
}
