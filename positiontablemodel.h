#ifndef POSITIONTABLEMODEL_H
#define POSITIONTABLEMODEL_H

#include <QSqlTableModel>


class PositionTableModel : public QSqlTableModel
{
public:
    PositionTableModel(QObject *parent=0, QSqlDatabase db = QSqlDatabase());
    virtual QVariant data(const QModelIndex & index, int role = Qt::EditRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    unsigned int checkableColNum = 0;
};

#endif // POSITIONTABLEMODEL_H
