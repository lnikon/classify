#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>

namespace QtCharts {
class QChartView;
class QPieSeries;
class QPieSlice;
class QChart;
}

class QAbstractItemModel;
class QHBoxLayout;

class ChartWidget : public QWidget
{
public:
    ChartWidget(QWidget* parent = nullptr);

    const QAbstractItemModel* pModel() const;
    void setPModel(const QAbstractItemModel* pModel);

    void prepare(const int columnIndex);

private:
    const QAbstractItemModel* m_pModel{nullptr};
    QtCharts::QChart* m_pChart{nullptr};
    QtCharts::QChartView* m_pView{nullptr};
    QtCharts::QPieSeries* m_pSeries{nullptr};
    QtCharts::QPieSlice* m_pSlice{nullptr};
    QHBoxLayout* m_pMainLayout;
};

#endif // CHARTWIDGET_H
