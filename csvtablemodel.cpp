#include "csvtablemodel.h"

#include <iostream>

std::size_t CsvTableModel::calledDataCnt_ = 0;

CsvTableModel::CsvTableModel(QObject *parent)
    : QAbstractTableModel(parent)
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

QVariant CsvTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant{};
    }

    const int row = index.row();
    if(row <= 0)
    {
        return QVariant{};
    }

    if(static_cast<std::size_t>(index.row()) >= mmcc_.rowCnt())
    {
        return QVariant{};
    }

    if(role == Qt::DisplayRole)
    {
        const std::string& result = mmcc_.data(static_cast<std::size_t>(index.row()), static_cast<std::size_t>(index.column()));
        return result.c_str();
    }

    return QVariant{};
}

bool CsvTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QVariant CsvTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CsvTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

bool CsvTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

MemoryMappedCsvContainer::ReturnCode CsvTableModel::loadFromFile(const QString &fileName, const QChar &delim)
{
    decltype (mmcc_)::ReturnCode ok = mmcc_.open(fileName.toStdString());

    // Set rows and columns count
    rowCnt_ = mmcc_.rowCnt();
    colCnt_ = mmcc_.colCnt();

    // Cache headers for further use
    const QString& header = mmcc_.row(0).c_str();
    headerNamesCached_ = header.split("\t");

    return ok;
}
