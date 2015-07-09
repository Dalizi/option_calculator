#ifndef ADDPARAMDIALOG_H
#define ADDPARAMDIALOG_H

#include "databaseaccess.h"

#include <QDialog>

namespace Ui {
class AddParamDialog;
}

class AddParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddParamDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~AddParamDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddParamDialog *ui;
    DatabaseAccess *db;

signals:
    void paramAdded();
};

#endif // ADDPARAMDIALOG_H
