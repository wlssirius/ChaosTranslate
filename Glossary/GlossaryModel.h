#ifndef GLOSSARY_MODEL_HEADER
#define GLOSSARY_MODEL_HEADER
#include <QAbstractTableModel>
#include <utility>
#include <vector>
class GlossaryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    using entry = std::pair<QString, QString>;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    GlossaryModel(QObject* parent = nullptr);
    int getGlossaryCount() { return m_glossary.size(); }
    void addEntry(QString original, QString translate);

    const std::vector<entry>& getGlossary() { return m_glossary; }
    void setGlossary(const std::vector<entry>& glossary);
    void setGlossary(std::vector<entry>&& glossary);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void clearAll();

signals:
    void deleteEntry(QString original, QString translate);

private:
    std::vector<entry> m_glossary;


};
#endif // GLOSSARY_MODEL_HEADER