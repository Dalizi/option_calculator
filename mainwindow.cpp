#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addorderdialog.h"
#include "transactionviewdialog.h"
#include "resetpassworddialog.h"
#include "adduserdialog.h"
#include "paramsettingdialog.h"

#include <QMessageBox>

MainWindow::MainWindow(DatabaseAccess *db, OptionValue *calc_server, CAccessRedis *redis, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(db),
    model(new PositionTableModel(this, db->getDatabase())),
    user_info_model(new PositionTableModel(this, db->getDatabase())),
    calc_server(calc_server),
    opt_calc(new OptionCalcDialog(calc_server, db)),
    redis(redis)
{
    ui->setupUi(this);
    init();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete opt_calc;
}

void MainWindow::init() {
    ui->optionClassComboBox->addItems(db->getAllClassCode());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    user_info_model->setTable("user_info");
    user_info_model->select();
    userInfoTable.setModel(user_info_model);
    updateRiskInfo();
    QAction *deleteAction = new QAction("Delte record", 0);
    ui->positionTableView->addAction(deleteAction);
    connect(ui->orderPlaceMenu, SIGNAL(triggered(QAction *)), this, SLOT(onOrderPlaceMenuTriggered(QAction *)));
    connect(ui->transactionMenu, SIGNAL(triggered(QAction *)), this, SLOT(onTransactionMenuTriggered(QAction *)));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteActionTriggered()));
    connect(ui->revertPushButton, SIGNAL(clicked(bool)), model, SLOT(revertAll()));
    connect(ui->optionPricingAction, SIGNAL(triggered()), opt_calc, SLOT(show()));
    connect(ui->actionReset_Password, SIGNAL(triggered()), this, SLOT(onResetPasswordActionTriggered()));
}

//void MainWindow::initPositionTable() {
//    model = new QSqlTableModel(this, db->getDatabase());
//    model->setTable("positions");
//    model->select();
//    QString filterText = "instr_code LIKE OTC-";
//    filterText += ui->optionClassComboBox->currentText();
//    filterText += '%';
//    model->setFilter(filterText);
//    model->setHeaderData(0, Qt::Horizontal, tr("Client ID"));
//    model->setHeaderData(1, Qt::Horizontal, tr("Instrument Code"));
//    model->setHeaderData(2, Qt::Horizontal, tr("Average Price"));
//    model->setHeaderData(3, Qt::Horizontal, tr("Total Amount"));
//    model->setHeaderData(4, Qt::Horizontal, tr("Availabel Amount"));
//    model->setHeaderData(5, Qt::Horizontal, tr("Frozen Amount"));
//    model->setHeaderData(6, Qt::Horizontal, tr("Long Short"));
//    model->setHeaderData(7, Qt::Horizontal, tr("Underlying Price"));
//    model->setHeaderData(8, Qt::Horizontal, tr("Occupied Margin"));
//    model->setHeaderData(9, Qt::Horizontal, tr("Underlying Code"));
//    model->setHeaderData(10, Qt::Horizontal, tr("Knock Out Price"));
//    model->setHeaderData(11, Qt::Horizontal, tr("Contract NO."));
//    ui->positionTableView->setModel(model);
//}

void MainWindow::onOrderPlaceMenuTriggered(QAction *action) {
    if (action == ui->orderPlaceAction) {
        auto opd = new addOrderDialog(db, redis, this);
        opd->show();
    }
}

void MainWindow::onTransactionMenuTriggered(QAction *action) {
    if (action == ui->transactionQueryAction) {
        auto tvd = new transactionViewDialog(db, this);
        tvd->show();
    }
}

void MainWindow::onTransactionWritten() {
    model->select();
}

void MainWindow::onDeleteActionTriggered() {
    if (!ui->positionTableView->currentIndex().isValid())
        return;
    int row = ui->positionTableView->currentIndex().row();
    ui->positionTableView->selectRow(row);
    model->removeRow(row);
}

void MainWindow::onRevertButtonClicked() {
    model->revertAll();
}

void MainWindow::on_savePushButton_clicked()
{

    if (QMessageBox::question(0, tr("Warning"), tr("Do you want to commit all changes?")) == QMessageBox::Yes)
        if (!model->submitAll())
            QMessageBox::warning(0, tr("Submission failed"), model->lastError().text());
}

void MainWindow::on_refreshPushButton_clicked()
{
    updateRiskInfo();
}

void MainWindow::updateRiskInfo() {
    PositionRisk ret;
    double pnl=0;
    auto positions = db->getAllPosition(ui->optionClassComboBox->currentText());
    for (auto p:positions) {
        auto pr = calc_server->PositionGreeks(p);
        pnl -= -calc_server->Position_PnL(p, false);
        ret.delta -= pr.delta;
        ret.delta_f -= pr.delta_f;
        ret.gamma -= pr.gamma;
        ret.theta -= pr.theta;
        ret.vega -= pr.vega;
    }
    ui->deltaLineEdit->setText(QString::number(ret.delta));
    ui->deltaFLineEdit->setText(QString::number(ret.delta_f));
    ui->gammaLineEdit->setText(QString::number(ret.gamma));
    ui->thetaLineEdit->setText(QString::number(ret.theta));
    ui->vegaLineEdit->setText(QString::number(ret.vega));
    ui->pnlLineEdit->setText(QString::number(pnl));
}

void MainWindow::on_optionClassComboBox_currentTextChanged(const QString &arg1)
{
    model->setTable("positions");
    QString filterText = "instr_code LIKE 'OTC-";
    filterText += arg1;
    filterText += "%'";
    model->setFilter(filterText);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Client ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Instrument Code"));
    model->setHeaderData(2, Qt::Horizontal, tr("Average Price"));
    model->setHeaderData(3, Qt::Horizontal, tr("Total Amount"));
    model->setHeaderData(4, Qt::Horizontal, tr("Availabel Amount"));
    model->setHeaderData(5, Qt::Horizontal, tr("Frozen Amount"));
    model->setHeaderData(6, Qt::Horizontal, tr("Long Short"));
    model->setHeaderData(7, Qt::Horizontal, tr("Underlying Price"));
    model->setHeaderData(8, Qt::Horizontal, tr("Occupied Margin"));
    model->setHeaderData(9, Qt::Horizontal, tr("Underlying Code"));
    model->setHeaderData(10, Qt::Horizontal, tr("Knock Out Price"));
    model->setHeaderData(11, Qt::Horizontal, tr("Contract NO."));
    ui->positionTableView->setModel(model);
}

void MainWindow::onResetPasswordActionTriggered() {
    ResetPasswordDialog rpd(db, this);
    rpd.exec();
}

void MainWindow::on_actionAdd_User_triggered() {
    AddUserDialog aud(db);
    aud.exec();
}

void MainWindow::on_actionUser_Info_triggered() {
    user_info_model->select();
    userInfoTable.show();
}

void MainWindow::on_actionParam_Setting_triggered() {
    ParamSettingDialog psd(db);
    psd.exec();
}
