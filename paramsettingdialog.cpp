#include "paramsettingdialog.h"
#include "ui_paramsettingdialog.h"

ParamSettingDialog::ParamSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamSettingDialog)
{
    ui->setupUi(this);
}

ParamSettingDialog::~ParamSettingDialog()
{
    delete ui;
}
