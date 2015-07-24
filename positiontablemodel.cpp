#include "positiontablemodel.h"

#include <QColor>
PositionTableModel::PositionTableModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db)
{

}

QVariant PositionTableModel::data(const QModelIndex & index, int role) const {
    if (role == Qt::BackgroundRole && isDirty(index)) {
        return QVariant::fromValue(QColor(Qt::red));
    } else if (role == Qt::CheckStateRole && index.column() == checkableColNum)
        return Qt::Checked;
    else {
        return QSqlTableModel::data(index, role);
    }
}

Qt::ItemFlags PositionTableModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QSqlTableModel::flags(index);
    if (index.column() != checkableColNum)
        return flags;
    else
        return flags | Qt::ItemIsUserCheckable;
}
