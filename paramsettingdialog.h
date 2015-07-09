#ifndef PARAMSETTINGDIALOG_H
#define PARAMSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ParamSettingDialog;
}

class ParamSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamSettingDialog(QWidget *parent = 0);
    ~ParamSettingDialog();

private:
    Ui::ParamSettingDialog *ui;
};

#endif // PARAMSETTINGDIALOG_H
