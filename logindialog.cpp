#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    db(db)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::accept() {
    bool is_success = db->connectToDatabase();
    auto username = ui->userNameLineEdit->text();
    auto password = ui->passwordLineEdit->text();
    db->setLoginInfo(username, password);
    if (is_success)
        QDialog::accept();
    else {
        QMessageBox::warning(this, "连接数据库失败", db->lastError());
        QDialog::reject();
    }

}
