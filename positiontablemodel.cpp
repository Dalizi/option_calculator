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

//Qt::ItemFlags PositionTableModel::flags(const QModelIndex &index) const {
//    if (index.column() != checkableColNum)
//        return QSqlTableModel::flags(index.sibling(index.row(), index.column() - 1)) | Qt::ItemIsEditable;
//    else
//        return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//}

//int PositionTableModel::columnCount(const QModelIndex & parent) const {
//    return QSqlTableModel::columnCount() + 1;
//}



//bool PositionTableModel::setData( const QModelIndex &index, const QVariant &value, int role )
//{
//        if(!index.isValid())
//                return false;

//        if (role == Qt::CheckStateRole && index.column() == 0)
//        {

//        } else {
//            return QSqlTableModel::setData(index, value, role);
//        }

//        return true;
//}
