#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include "memorymappedcsvcontainer.h"

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include <variant>
#include <optional>
#include <utility>
#include <memory>
#include <QThread>

using ComboboxValues    = QVector<QString>;
using CellValueVariant  = QVariant;

enum class CellValueType
{
    SINGLE = 1,
    COMBOBOX = 2,

    UNKOWN,
    CELL_VALUE_TYPE_END
};

enum class CellValueType_SingleTag {};
enum class CellValueType_ComboboxTag {};

struct CellValue
{
    CellValueType type_{CellValueType::UNKOWN};

    CellValue() = default;

    explicit CellValue(const QString& value)
        : type_{CellValueType::SINGLE},
          value_{value}
    { }

    explicit CellValue(const std::string& value)
        : type_{CellValueType::SINGLE},
          value_{value.c_str()}
    { }

    explicit CellValue(const QStringList& value)
        : type_{CellValueType::COMBOBOX},
          value_{value}
    { }

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

    std::optional<QStringList> get()
    {
        return std::nullopt;
    }

    std::optional<QStringList> get(CellValueType_SingleTag)
    {
        if(value_.index() == static_cast<std::size_t>(CellValueType::SINGLE))
        {
            return QStringList{std::get<static_cast<std::size_t>
                        (CellValueType::SINGLE)>(value_)};
        }

        return std::optional<QStringList>(std::nullopt);
    }

    std::optional<QStringList> get(CellValueType_ComboboxTag)
    {
        if(value_.index() == static_cast<std::size_t>(CellValueType::COMBOBOX))
        {
            return std::get<static_cast<std::size_t>
                    (CellValueType::COMBOBOX)>(value_);
        }

        return std::optional<QStringList>(std::nullopt);
    }

private:
    std::variant<std::monostate, QString, QStringList> value_;
};

struct GenericRow
{
    void set(std::size_t idx, const CellValue& value)
    {
        if(idx >= colCnt_) { return; }

        //        if(idx >= values_.size())
        //        {
        values_.insert(idx, value);
        //        }

        //        values_[static_cast<int>(idx)] = value;
    }

    std::optional<CellValue> get(std::size_t idx)
    {
        if(idx >= colCnt_) { return std::nullopt; }

        if(idx >= static_cast<std::size_t>(values_.size()))
        {
            return std::nullopt;
        }

        return values_.at(static_cast<int>(idx));
    }

    void reserve(std::size_t capacity)
    {
        colCnt_ = capacity;
        values_.reserve(static_cast<int>(colCnt_));
    }

    void setColumnsCount(std::size_t colCnt) { colCnt_ = colCnt; }
    std::size_t getColumnsCount() { return colCnt_; }

private:
    QVector<CellValue>  values_{};
    std::size_t         colCnt_{0};
};
using GenericTable = QVector<GenericRow>;


struct DataLoaderThread : public QThread
{
public:
    DataLoaderThread(std::vector<std::vector<std::string>>& dataCache)
        : dataCache(dataCache) {}

    void run() override;

    int rowCnt;
    int colCnt;
    std::vector<std::vector<std::string>>& dataCache;
    MemoryMappedCsvContainer* mmcc;
};

class CsvTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CsvTableModel(QObject* parent = Q_NULLPTR);

    int columnCount(const QModelIndex &parent) const override;

    int rowCount(const QModelIndex &parent) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index,
                  int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    MemoryMappedCsvContainer::ReturnCode loadFromFile(const QString &fileName,
                                                      const QChar &delim = ',');

    void loadDataIntoCache();

signals:
    void numberPopulated(int);

private:
    MemoryMappedCsvContainer    mmcc_{};
    mutable QStringList         headerNamesCached_{};

    std::size_t                 rowCnt_{0};
    std::size_t                 colCnt_{0};

    const std::size_t           batchSize_{100};

    static uintmax_t            calledDataCnt_;

    mutable std::vector<std::vector<std::string>>   dataCache_;

    std::shared_ptr<GenericTable> m_pGenericTable{nullptr};

    void loadDataIntoCacheWorker(int rowCnt,
                                 int colCnt,
                                 std::vector<std::vector<std::string>>& dataCache,
                                 MemoryMappedCsvContainer& mmcc);
};

#endif // CSVTABLEMODEL_H
