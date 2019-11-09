#include "csvtablemodel.h"

#include <iostream>
#include <thread>

#include <QDebug>

std::size_t CsvTableModel::calledDataCnt_ = 0;

CsvTableModel::CsvTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_pGenericTable{std::make_shared<GenericTable>()}
{

}

int CsvTableModel::columnCount(const QModelIndex &) const
{
    return static_cast<int>(colCnt_);
}

int CsvTableModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(rowCnt_);
}

Qt::ItemFlags CsvTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant CsvTableModel::data(const QModelIndex &index,
                             int role) const
{
    if(!index.isValid())
    {
        return QVariant{};
    }

    const int row = index.row();
    if(row < 0)
    {
        return QVariant{};
    }

    if(static_cast<std::size_t>(index.row()) >= mmcc_.rowCnt())
    {
        return QVariant{};
    }

    if(role == Qt::DisplayRole)
    {
        const int row = index.row();
        const int column = index.column();

        if(row < dataCache_.size()
           && column < dataCache_[row].size())
        {
            return dataCache_[row][column].c_str();
        }
    }

    return QVariant{};
}

bool CsvTableModel::setData(const QModelIndex &index,
                            const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

QVariant CsvTableModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant{};
    }

    if(orientation == Qt::Horizontal)
    {
        if(!headerNamesCached_.empty())
        {
            return headerNamesCached_.at(section);
        }
    }

    return QVariant{};
}

bool CsvTableModel::insertRows(int row, int count,
                               const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}

bool CsvTableModel::removeRows(int row, int count,
                               const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}

MemoryMappedCsvContainer::ReturnCode
CsvTableModel::loadFromFile(const QString &fileName,
                            const QChar &delim)
{
    m_pGenericTable->clear();

    MemoryMappedCsvContainer::ReturnCode ok
            = mmcc_.open(fileName.toStdString(), delim.toLatin1());

    /*
     *  Set rows and columns count
     */
    rowCnt_ = mmcc_.rowCnt();
    colCnt_ = mmcc_.colCnt();

    /*
     * Cache headers for further use
     */
    const QString& header = mmcc_.row(0).c_str();
    headerNamesCached_ = header.split(delim);

    loadDataIntoCache();

    return ok;
}

void CsvTableModel::loadDataIntoCacheWorker(int rowCnt,
                                            int colCnt,
                                            std::vector<std::vector<std::string>>& dataCache,
                                            MemoryMappedCsvContainer& mmcc)
{
    mmcc.data(dataCache);
}

void CsvTableModel::loadDataIntoCache()
{
    DataLoaderThread dataLoader(dataCache_);
    dataLoader.rowCnt = rowCnt_;
    dataLoader.colCnt = colCnt_;
    dataLoader.mmcc = &mmcc_;

    connect(&dataLoader, &DataLoaderThread::finished, this, [&dataLoader, this]()
    {
        this->mmcc_.reset();
    });

    dataLoader.run();
}

void DataLoaderThread::run()
{
    mmcc->data(dataCache);
}
