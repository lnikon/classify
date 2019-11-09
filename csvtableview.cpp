#include "csvtableview.h"

#include <QSortFilterProxyModel>
#include <QHeaderView>

CSVTableView::CSVTableView(QWidget *parent)
    : QTableView(parent),
      m_pProxyModel(new QSortFilterProxyModel)
{
    QTableView::setSortingEnabled(true);

    QHeaderView* pHeaderView = QTableView::horizontalHeader();
    pHeaderView->setSectionResizeMode(QHeaderView::Stretch);
}

void CSVTableView::setModel(QAbstractItemModel *model)
{
//    QTableView::setModel(model);

    m_pProxyModel->setSourceModel(model);

    QTableView::setModel(m_pProxyModel);
}
