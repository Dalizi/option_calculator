#include "positiontablemodel.h"

#include <QColor>
PositionTableModel::PositionTableModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db)
{

}

QVariant PositionTableModel::data(const QModelIndex & index, int role) const {
    if (role == Qt::BackgroundRole && isDirty(index)) {
        return QVariant::fromValue(QColor(Qt::red));
    } else {
        return QSqlTableModel::data(index, role);
    }
}

