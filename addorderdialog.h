#ifndef ADDORDERDIALOG_H
#define ADDORDERDIALOG_H

#include "databaseaccess.h"
#include "accessredis.h"
#include <QDialog>

namespace Ui {
class addOrderDialog;
}

class addOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addOrderDialog(DatabaseAccess *db, CAccessRedis *redis, QWidget *parent = 0);
    ~addOrderDialog();
    virtual void accept();

private:
    Ui::addOrderDialog *ui;
    DatabaseAccess *db;
    CAccessRedis *redis;

    void init();
};

#endif // ADDORDERDIALOG_H
