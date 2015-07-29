#ifndef POSITIONTABLEMODEL_H
#define POSITIONTABLEMODEL_H

#include <QSqlTableModel>


class PositionTableModel : public QSqlTableModel
{
public:
    PositionTableModel(QObject *parent=0, QSqlDatabase db = QSqlDatabase());
    virtual QVariant data(const QModelIndex & index, int role = Qt::EditRole) const;
//    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
//    virtual bool setData( const QModelIndex &index, const QVariant &value, int role );
private:
//    unsigned int checkableColNum = 0;
//    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
};

#endif // POSITIONTABLEMODEL_H
