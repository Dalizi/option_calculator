#include "adduserdialog.h"
#include "ui_adduserdialog.h"

#include <QMessageBox>

AddUserDialog::AddUserDialog(DatabaseAccess *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog),
    db(db)
{
    ui->setupUi(this);
    ui->privilegesComboBox->addItem("Admin");
    ui->privilegesComboBox->addItem("Broker");
    ui->privilegesComboBox->addItem("Quant");
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

void AddUserDialog::on_buttonBox_accepted()
{
    auto username = ui->userNameLineEdit->text();
    auto passwd = ui->initialPasswordLineEdit->text();
    DatabaseAccess::UserPrivilege priv;
    if (ui->privilegesComboBox->currentText() == "Admin")
        priv = DatabaseAccess::ADMIN;
    else if (ui->privilegesComboBox->currentText() == "Broker")
        priv = DatabaseAccess::BROKER;
    else if (ui->privilegesComboBox->currentText() == "Quant")
        priv = DatabaseAccess::QUANT;
    else
        QMessageBox::warning(this, "Error", "Unknown user type.");

    if (!db->addUser(username, passwd, priv))
        QMessageBox::warning(this, "Error", "Creating user failed.");

}
