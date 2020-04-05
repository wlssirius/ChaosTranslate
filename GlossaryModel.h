#ifndef MYMODEL_H
#define MYMODEL_H
#include <QAbstractTableModel>
#include <utility>
#include <vector>
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
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int getGlossaryCount() { return m_glossary.size(); }
    void addEntry(QString original, QString translate);

private:
    using entry = std::pair<QString, QString>;
    std::vector<entry> m_glossary;


};
#endif // MYMODEL_H