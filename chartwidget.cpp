#include "chartwidget.h"

#include <QHBoxLayout>
#include <QAbstractItemModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QChart>
#include <QDebug>

ChartWidget::ChartWidget(QWidget* parent)
    : QWidget(parent),
      m_pChart(new QtCharts::QChart),
      m_pView(new QtCharts::QChartView(m_pChart)),
      m_pSeries(new QtCharts::QPieSeries)
{
    m_pChart->legend()->setAlignment(Qt::AlignRight);
    m_pChart->addSeries(m_pSeries);
    m_pView->setRenderHint(QPainter::Antialiasing);
    m_pView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);

    m_pMainLayout = new QHBoxLayout;
    m_pMainLayout->addWidget(m_pView);
    setLayout(m_pMainLayout);
}

const QAbstractItemModel* ChartWidget::pModel() const
{
    return m_pModel;
}

void ChartWidget::setPModel(const QAbstractItemModel* pModel)
{
    m_pModel = pModel;
}

void ChartWidget::prepare(const int columnIndex)
{
    using namespace QtCharts;

    const auto rowCnt = m_pModel->rowCount();

//    m_pSeries = new QPieSeries;
//    m_pSeries->setLabelsPosition(QPieSlice::LabelInsideTangential);

    QMap<QString, int> valueCount;
    for (int i = 0; i < rowCnt; ++i)
    {
        QModelIndex index = m_pModel->index(i, columnIndex, QModelIndex());
        QString key = index.data().toString();

        if (valueCount.contains(key))
        {
            valueCount[key]++;
        }
        else
        {
            valueCount[key] = 1;
        }
    }

    for (auto it = valueCount.begin(); it != valueCount.end(); ++it)
    {
        m_pSeries->append(it.key(), it.value());
    }

    auto slices = m_pSeries->slices();
    foreach( auto* slice, slices)
    {
        connect(slice, QOverload<bool>::of(&QtCharts::QPieSlice::hovered), this,
                [=](const bool state)
        {
            slice->setLabelVisible(state);
        });
    }

//    m_pChart->addSeries(m_pSeries);
}
