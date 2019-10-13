#ifndef CSVTABLEVIEW_H
#define CSVTABLEVIEW_H

#include <QTableView>

class QAbstractTableModel;
class CSVTableView : public QTableView
{
    Q_OBJECT
public:
    CSVTableView(QWidget* parent = nullptr);

    void setModel(QAbstractItemModel* model) override;
};

#endif // CSVTABLEVIEW_H
