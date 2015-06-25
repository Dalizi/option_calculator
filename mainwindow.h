#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databaseaccess.h"

#include <QMainWindow>

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

    void initPositionTable();

public slots:
    void onOrderPlaceMenuTriggered(QAction *);
    void onTransactionMenuTriggered(QAction *);
};

#endif // MAINWINDOW_H
