#include "customchartview.h"

CustomChartView::CustomChartView(QChart* pChart)
    : QChartView(pChart)
{

}

//bool CustomChartView::viewportEvent(QEvent* event)
//{

//}

//void CustomChartView::mousePressEvent(QMouseEvent* event)
//{

//}

//void CustomChartView::mouseMoveEvent(QMouseEvent* event)
//{

//}

//void CustomChartView::mouseReleaseEvent(QMouseEvent* event)
//{

//}

void CustomChartView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
            chart()->zoomIn();
            break;
        case Qt::Key_Minus:
            chart()->zoomOut();
            break;
    }
}
