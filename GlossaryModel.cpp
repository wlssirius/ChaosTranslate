
#include "GlossaryModel.h"
GlossaryModel::GlossaryModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}
int GlossaryModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_glossary.size();
}
int GlossaryModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}
QVariant GlossaryModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto& e = m_glossary[index.row()];
        if (index.column() == 0)
        {
            return e.first;
        }
        else if (index.column() == 1)
        {
            return e.second;
        }
    }
    return QVariant();
}

QVariant GlossaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation==Qt::Orientation::Horizontal)
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

bool GlossaryModel::insertRows(int row, int count, const QModelIndex& parent)
{ 
    beginInsertRows(parent, m_glossary.size(), m_glossary.size());
    m_glossary.emplace_back();
    endInsertRows();
    return false;
}

bool GlossaryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;
        //save value from editor to member m_gridData
        auto& e = m_glossary[index.row()];
        if (index.column() == 0)
        {
            e.first = value.toString();
        }
        else if(index.column() == 1)
        {
            e.second = value.toString();
        }

        //emit editCompleted(result);
        return true;
    }
    return false;
}

Qt::ItemFlags GlossaryModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void GlossaryModel::addNewRow()
{
    insertRow(m_glossary.size());
}

void GlossaryModel::deleteRow()
{
}
