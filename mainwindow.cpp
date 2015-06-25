#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addorderdialog.h"
#include "transactionviewdialog.h"

#include <QSqlTableModel>

MainWindow::MainWindow(DatabaseAccess *db, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(db)
{
    ui->setupUi(this);
    initPositionTable();
    connect(ui->orderPlaceMenu, SIGNAL(triggered(QAction *)), this, SLOT(onOrderPlaceMenuTriggered(QAction *)));
    connect(ui->transactionMenu, SIGNAL(triggered(QAction *)), this, SLOT(onTransactionMenuTriggered(QAction *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initPositionTable() {
    auto model = new QSqlTableModel(this, db->getDatabase());
    model->setTable("positions");
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

void MainWindow::onOrderPlaceMenuTriggered(QAction *action) {
    if (action == ui->orderPlaceAction) {
        auto opd = new addOrderDialog(db);
        opd->show();
    }
}

void MainWindow::onTransactionMenuTriggered(QAction *action) {
    if (action == ui->transactionQueryAction) {
        auto tvd = new transactionViewDialog(db);
        tvd->show();
    }
}
