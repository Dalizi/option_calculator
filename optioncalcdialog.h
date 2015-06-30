#ifndef OPTIONCALCDIALOG_H
#define OPTIONCALCDIALOG_H

#include <QDialog>

#include "valuation_class.h"

namespace Ui {
class OptionCalcDialog;
}

class OptionCalcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionCalcDialog(OptionValue *calc_server, QWidget *parent = 0);
    ~OptionCalcDialog();

private slots:
    void on_pricingPushButton_clicked();

private:
    Ui::OptionCalcDialog *ui;
    OptionValue *calc_server;

    void init();
};

#endif // OPTIONCALCDIALOG_H
