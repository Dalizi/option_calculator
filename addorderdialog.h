#ifndef ADDORDERDIALOG_H
#define ADDORDERDIALOG_H

#include "databaseaccess.h"
#include <QDialog>

namespace Ui {
class addOrderDialog;
}

class addOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addOrderDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~addOrderDialog();
    virtual void accept();

private:
    Ui::addOrderDialog *ui;
    DatabaseAccess *db;

    void init();
};

#endif // ADDORDERDIALOG_H
