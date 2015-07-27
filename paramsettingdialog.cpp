#include "paramsettingdialog.h"
#include "ui_paramsettingdialog.h"
#include "addparamdialog.h"

#include <QMessageBox>

ParamSettingDialog::ParamSettingDialog(DatabaseAccess *db, OptionValue *calc_server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamSettingDialog),
    db(db),
    model(new PositionTableModel(this, db->getDatabase())),
    calc_server(calc_server)
{
    ui->setupUi(this);
    init();
}

ParamSettingDialog::~ParamSettingDialog()
{
    delete ui;
}

void ParamSettingDialog::init() {
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("param");
    model->select();
    ui->paramTableView->setModel(model);
    QAction *deleteAction = new QAction("Delte record", 0);
    ui->paramTableView->addAction(deleteAction);
    connect(ui->savePushButton, SIGNAL(clicked()), this, SLOT(onSummitButtonClicked()));
    connect(ui->undoPushButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteActionTriggered()));
}

void ParamSettingDialog::onSummitButtonClicked() {
    if (QMessageBox::question(0, tr("Warning"), tr("Do you want to commit all changes?")) == QMessageBox::Yes) {
        if (!model->submitAll())
            QMessageBox::warning(0, tr("Submission failed"), model->lastError().text());
    } else {
        calc_server->Parameters_Setting(db->getParam());
    }
}

void ParamSettingDialog::onDeleteActionTriggered() {
    if (!ui->paramTableView->currentIndex().isValid())
        return;
    int row = ui->paramTableView->currentIndex().row();
    ui->paramTableView->selectRow(row);
    model->removeRow(row);
}

void ParamSettingDialog::onParamAdded() {
    model->select();
}

void ParamSettingDialog::on_addPushButton_clicked()
{
    AddParamDialog apd(db);
    connect(&apd, SIGNAL(paramAdded()), this, SLOT(onParamAdded()));
    apd.exec();
}
