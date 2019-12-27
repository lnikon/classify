#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QObject>
#include <QtCharts/QChartView>
using namespace QtCharts;

class CustomChartView : public QChartView
{
public:
    CustomChartView(QChart* pChart);

protected:
//    bool viewportEvent(QEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // CUSTOMCHARTVIEW_H
