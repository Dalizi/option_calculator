#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databaseaccess.h"

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseAccess *db, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseAccess *db;
    QSqlTableModel *model;

    void initPositionTable();

public slots:
    void onOrderPlaceMenuTriggered(QAction *);
    void onTransactionMenuTriggered(QAction *);
    void onTransactionWritten();

private slots:
    void onDeleteActionTriggered();
    void onRevertButtonClicked();
    void on_savePushButton_clicked();
};

#endif // MAINWINDOW_H
