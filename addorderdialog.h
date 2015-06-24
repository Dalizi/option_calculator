#ifndef ADDORDERDIALOG_H
#define ADDORDERDIALOG_H

#include <QDialog>

namespace Ui {
class addOrderDialog;
}

class addOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addOrderDialog(QWidget *parent = 0);
    ~addOrderDialog();

private:
    Ui::addOrderDialog *ui;
};

#endif // ADDORDERDIALOG_H
