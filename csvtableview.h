#ifndef CSVTABLEVIEW_H
#define CSVTABLEVIEW_H

#include <QTableView>

class QAbstractTableModel;
class QSortFilterProxyModel;
class QMouseEvent;
class QMenu;
class CSVTableView : public QTableView
{
    Q_OBJECT

public:
    CSVTableView(QWidget* parent = nullptr);

    void setModel(QAbstractItemModel* model) override;
    void onUniqueCountAction();

    /*
     * Public slots
     */
public slots:
    void slotSectionClicked(int logicalIndex);
    void onCustomContextMenu(const QPoint& crPoint);

    /*
     * Private methods
     */
private:
    void mouseReleaseEvent(QMouseEvent* pEvent) override;

    /*
     * Private properties
     */
private:
    QSortFilterProxyModel* m_pProxyModel{nullptr};
    QMenu* m_pContextMenu;
};

#endif // CSVTABLEVIEW_H
