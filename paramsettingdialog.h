#ifndef PARAMSETTINGDIALOG_H
#define PARAMSETTINGDIALOG_H
#include "databaseaccess.h"
#include "positiontablemodel.h"

#include <QDialog>

namespace Ui {
class ParamSettingDialog;
}

class ParamSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamSettingDialog(DatabaseAccess *db, QWidget *parent = 0);
    ~ParamSettingDialog();

private:
    Ui::ParamSettingDialog *ui;
    DatabaseAccess *db;
    PositionTableModel *model;

    void init();

private slots:
    void onDeleteActionTriggered();
    void onSummitButtonClicked();
    void onParamAdded();
    void on_addPushButton_clicked();
};

#endif // PARAMSETTINGDIALOG_H
