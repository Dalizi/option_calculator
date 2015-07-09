#ifndef ADDPARAMDIALOG_H
#define ADDPARAMDIALOG_H

#include <QDialog>

namespace Ui {
class AddParamDialog;
}

class AddParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddParamDialog(QWidget *parent = 0);
    ~AddParamDialog();

private:
    Ui::AddParamDialog *ui;
};

#endif // ADDPARAMDIALOG_H
