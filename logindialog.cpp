#include "logindialog.h"
#include "databaseoperation.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::accept() {
    auto username = ui->userNameLineEdit->text();
    auto password = ui->passwordLineEdit->text();
    DatabaseOperation::setLoginInfo(username, password);
    QDialog::accept();

}
