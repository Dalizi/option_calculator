#include "resetpassworddialog.h"
#include "ui_resetpassworddialog.h"

#include <QMessageBox>

ResetPasswordDialog::ResetPasswordDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetPasswordDialog),
    db(db)
{
    ui->setupUi(this);
}

ResetPasswordDialog::~ResetPasswordDialog()
{
    delete ui;
}

void ResetPasswordDialog::on_buttonBox_accepted()
{
    if (ui->newPasswordLineEdit->text() != ui->reenterNewPasswordLineEdit->text()) {
        QMessageBox::warning(this, tr("Error"), tr("Two old passwords do not match."));
        return;
    }
    if (db->setPassword(ui->newPasswordLineEdit->text(), ui->oldPasswordLineEdit->text())) {
        QMessageBox::about(this, tr("Success"), tr("Password reseted."));
        accept();
    }
}
