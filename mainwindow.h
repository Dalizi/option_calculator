#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databaseaccess.h"
#include "optioncalcdialog.h"
#include "accessredis.h"

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseAccess *db, OptionValue *calc_server, CAccessRedis *redis, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseAccess *db;
    QSqlTableModel *model;
    OptionValue *calc_server;
    OptionCalcDialog *opt_calc;
    CAccessRedis *redis;

    void init();
   // void initPositionTable();
    void updateRiskInfo();

public slots:
    void onOrderPlaceMenuTriggered(QAction *);
    void onTransactionMenuTriggered(QAction *);
    void onTransactionWritten();
    //void onOptionPricingActionTriggered();

private slots:
    void onDeleteActionTriggered();
    void onRevertButtonClicked();
    void on_savePushButton_clicked();
    void on_refreshPushButton_clicked();
    void on_optionClassComboBox_currentTextChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
