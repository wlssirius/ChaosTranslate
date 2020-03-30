
#include "GlossaryModel.h"
GlossaryModel::GlossaryModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    setHeaderData(1, Qt::Orientation::Horizontal, QString("Test"));
}
int GlossaryModel::rowCount(const QModelIndex& /*parent*/) const
{
    return 2;
}
int GlossaryModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}
QVariant GlossaryModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return QString("Row%1, Column%2")
        .arg(index.row() + 1)
        .arg(index.column() + 1);
    return QVariant();
}

QVariant GlossaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return QString("Index");
        case 1:
            return QString("Text");
        case 2:
            return QString("Translate");
        default:
            return QVariant();
        }
    }
    return QVariant();
}
