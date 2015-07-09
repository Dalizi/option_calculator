#include "addparamdialog.h"
#include "ui_addparamdialog.h"

AddParamDialog::AddParamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddParamDialog)
{
    ui->setupUi(this);
}

AddParamDialog::~AddParamDialog()
{
    delete ui;
}
