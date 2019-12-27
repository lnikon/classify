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

    QAction* pUniqueCountAction = new QAction("Unique values in row", m_pContextMenu);
    connect(pUniqueCountAction, &QAction::triggered, this, &CSVTableView::onUniqueCountAction);
    m_pContextMenu->addAction(pUniqueCountAction);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &CSVTableView::customContextMenuRequested, this, &CSVTableView::onCustomContextMenu);
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
    using namespace QtCharts;
    QModelIndex index = currentIndex();
    if (!index.isValid())
    {
        return;
    }

    const auto colIdx = index.column();
    QAbstractItemModel* pSelectionModel = selectionModel()->model();
    const auto rowCnt = pSelectionModel->rowCount();

    ClassificationDialog* dlg = new ClassificationDialog();
    dlg->setModel(pSelectionModel);
    dlg->show();

//    QPieSeries* pSeries = new QPieSeries;
//    pSeries->setLabelsPosition(QPieSlice::LabelInsideTangential);

//    QMap<QString, int> valueCount;
//    for (int i = 0; i < rowCnt; ++i)
//    {
//        QModelIndex index = pSelectionModel->index(i, colIdx, QModelIndex());
//        QString key = index.data().toString();

//        if (valueCount.contains(key))
//        {
//            valueCount[key]++;
//        }
//        else
//        {
//            valueCount[key] = 0;
//        }
//    }

//    for (auto it = valueCount.begin(); it != valueCount.end(); ++it)
//    {
//        pSeries->append(it.key(), it.value());
//    }

//    auto slices = pSeries->slices();
//    foreach( auto* slice, slices)
//    {
//        slice->setLabelVisible();
//    }

//    QChart* pChart = new QChart;
//    pChart->addSeries(pSeries);
//    pChart->legend()->hide();

//    QtCharts::QChartView* pView = new QtCharts::QChartView(pChart);
////    CustomChartView* pView = new CustomChartView(pChart);
//    pView->setRenderHint(QPainter::Antialiasing);
//    pView->setRubberBand(QChartView::HorizontalRubberBand);

//    QHBoxLayout* pHLayout = new QHBoxLayout;
//    pHLayout->addWidget(pView);

//    QWidget* pWgt = new QWidget;
//    pWgt->resize(600, 800);
//    pWgt->setLayout(pHLayout);
//    pWgt->show();
}
