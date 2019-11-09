#ifndef CSVTABLEVIEW_H
#define CSVTABLEVIEW_H

#include <QTableView>

class QAbstractTableModel;
class QSortFilterProxyModel;
class CSVTableView : public QTableView
{
    Q_OBJECT

public:
    CSVTableView(QWidget* parent = nullptr);

    void setModel(QAbstractItemModel* model) override;

private:
    QSortFilterProxyModel* m_pProxyModel{nullptr};
};

#endif // CSVTABLEVIEW_H
