#include "csvtableview.h"

CSVTableView::CSVTableView(QWidget *parent)
    : QTableView(parent)
{

}

void CSVTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
}
