/*
*   Copyright (C) 2019-2020  Wei Lisi (Willis) <weilisi16@gmail.com>
*	This file is part of ChaosTranslate
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GlossaryModel.h"
#include "QMessageBox"
#include "algorithm"
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
            return tr("Text");
        case 1:
            return tr("Translate");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool GlossaryModel::insertRows(int row, int count, const QModelIndex& parent)
{ 
    beginInsertRows(parent, m_glossary.size(), m_glossary.size());
    //m_glossary.emplace_back();
    endInsertRows();
    return false;
}

bool GlossaryModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (count == 0)
    {
        return false;
    }
    beginRemoveRows(parent, row, row);
    for (int i = 0; i < count; i++)
    {
        emit deleteEntry(m_glossary[row+i].first, m_glossary[row+i].second);
    }
    m_glossary.erase(m_glossary.begin() + row);
    endRemoveRows();
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
            if (value.toString().size() == 0)
            {
                QMessageBox msg;
                msg.setText(tr("Original Text shouldn't be empty"));
                msg.exec();
                return false;
            }
            e.first = value.toString();
        }
        else if(index.column() == 1)
        {
            e.second = value.toString();
        }

        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags GlossaryModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void GlossaryModel::clearAll()
{
    int rows = rowCount();
    removeRows(0, rows);
    m_glossary.clear();
}

void GlossaryModel::addEntry(QString original, QString translate)
{
    int idx = -1;
    for (int i = 0; i < m_glossary.size(); i++)
    {
        if (m_glossary[i].first == original)
        {
            idx = i;
            break;
        }
    }
    if (idx==-1)
    {
        insertRow(getGlossaryCount());
        m_glossary.emplace_back(original, translate);
    }
    else
    {
        m_glossary[idx].second = translate;
        setData(createIndex(idx, 1), translate);
    }
}

void GlossaryModel::setGlossary(const std::vector<entry>& glossary)
{
    clearAll();
    m_glossary = glossary;
    for (int i = 0; i < m_glossary.size(); i++)
    {
        insertRow(i);
    }
}

void GlossaryModel::setGlossary(std::vector<entry>&& glossary)
{
    clearAll();
    m_glossary = glossary;
    for (int i = 0; i < m_glossary.size(); i++)
    {
        insertRow(i);
    }
}



