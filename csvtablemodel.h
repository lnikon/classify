#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include "memorymappedcsvcontainer.h"

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include <variant>
#include <optional>

using ComboboxValues    = QVector<QString>;
using CellValueVariant  = QVariant;

enum class CellValueType
{
    SINGLE = 0,
    COMBOBOX = 1,

    UNKOWN,
    CELL_VALUE_TYPE_END
};

enum class CellValueType_SingleTag {};
enum class CellValueType_ComboboxTag {};

struct CellValue
{
    CellValueType       type_{CellValueType::UNKOWN};

    CellValue(const CellValue& other)
        : type_{other.type_},
          value_{other.value_}
    { }

    CellValue& operator=(const CellValue& other)
    {
        if(this == &other) { return *this; }

        type_ = other.type_;
        value_ = other.value_;

        return *this;
    }

    template <class T>
    std::optional<T> get() { return std::nullopt; }

    template <CellValueType_SingleTag>
    std::optional<QString> get()
    {
        if(std::holds_alternative<QString>(value_))
        {
            return std::get<static_cast<std::size_t>(CellValueType::SINGLE)>(value_);
        }

        return std::nullopt;
    }

    template <CellValueType_ComboboxTag>
    std::optional<QStringList> get()
    {
        if(std::holds_alternative<QStringList>(value_))
        {
            return std::get<static_cast<std::size_t>(CellValueType::COMBOBOX)>(value_);
        }

        return std::nullopt;
    }

private:
    std::variant<QString, QStringList> value_;
};

struct GenericRow
{
    void set(std::size_t idx, const CellValue& value)
    {
        if(idx >= colCnt_) { return; }

        values_[static_cast<int>(idx)] = value;
    }

    std::optional<CellValue> get(std::size_t idx)
    {
        if(idx >= colCnt_) { return std::nullopt; }

        return values_.at(static_cast<int>(idx));
    }

    void setColumnsCount(std::size_t colCnt) { colCnt_ = colCnt; }
    std::size_t getColumnsCount() { return colCnt_; }

private:
    QVector<CellValue>  values_{};
    std::size_t         colCnt_{0};
};

class CsvTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CsvTableModel(QObject* parent = Q_NULLPTR);

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    MemoryMappedCsvContainer::ReturnCode loadFromFile(const QString &fileName, const QChar &delim = 0);

signals:
    void numberPopulated(int);

private:
    MemoryMappedCsvContainer    mmcc_{};
    mutable QStringList         headerNamesCached_{};

    std::size_t                 rowCnt_{0};
    std::size_t                 colCnt_{0};

    const std::size_t           batchSize_{100};

    static std::size_t          calledDataCnt_;
};

#endif // CSVTABLEMODEL_H
