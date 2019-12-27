#include "csvtableview.h"
#include "customchartview.h"
#include "classificationdialog.h"

#include <QMap>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QDebug>

#include <iterator>

CSVTableView::CSVTableView(QWidget *parent)
    : QTableView(parent),
      m_pProxyModel(new QSortFilterProxyModel),
      m_pContextMenu(new QMenu(this))
{
    QTableView::setSortingEnabled(true);
    QTableView::setSelectionBehavior(QAbstractItemView::SelectRows);

    QHeaderView* pHeaderView = QTableView::horizontalHeader();
    pHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    pHeaderView->setSortIndicatorShown(true);
    connect(pHeaderView, &QHeaderView::sectionClicked, this, &CSVTableView::slotSectionClicked);

//    QAction* pUniqueCountAction = new QAction("Unique values in row", m_pContextMenu);
//    connect(pUniqueCountAction, &QAction::triggered, this, &CSVTableView::onUniqueCountAction);
//    m_pContextMenu->addAction(pUniqueCountAction);

//    this->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(this, &CSVTableView::customContextMenuRequested, this, &CSVTableView::onCustomContextMenu);
}

void CSVTableView::setModel(QAbstractItemModel *model)
{
    m_pProxyModel->setSourceModel(model);

    QTableView::setModel(m_pProxyModel);
}

void CSVTableView::slotSectionClicked(int logicalIndex)
{

}

void CSVTableView::onCustomContextMenu(const QPoint& crPoint)
{
    QModelIndex index = indexAt(crPoint);
    if (!index.isValid())
    {
        return;
    }

    m_pContextMenu->exec(viewport()->mapToGlobal(crPoint));
}

void CSVTableView::mouseReleaseEvent(QMouseEvent* pEvent)
{
    QTableView::mouseReleaseEvent(pEvent);

    if (pEvent->button() == Qt::RightButton)
    {
        if (!selectionModel())
        {
            return;
        }

        QModelIndexList selection = selectionModel()->selectedRows();

        QVector<QVariant> data;

        foreach (QModelIndex index, selection)
        {

        }
    }
}

void CSVTableView::onUniqueCountAction()
{
    QAbstractItemModel* pSelectionModel = selectionModel()->model();

    ClassificationDialog* dlg = new ClassificationDialog();
    dlg->setModel(pSelectionModel);
    dlg->show();

}
