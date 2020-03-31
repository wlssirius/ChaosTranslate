#ifndef MYMODEL_H
#define MYMODEL_H
//! [Quoting ModelView Tutorial]
// mymodel.h
#include <QAbstractTableModel>
class GlossaryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    GlossaryModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    void addNewRow();

private:
    int m_rowCount = 2;


};
//! [Quoting ModelView Tutorial]
#endif // MYMODEL_H