#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

enum ActionIndex
{
    OPEN_TSV = 0,
    SAVE,
    KNN,
    CLASSIFY,
    ENUM_SIZE
};

class CSVTableView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui{nullptr};
    CSVTableView* csvTableView_{nullptr};
    QVector<QAction *> actions_;

    void setupTop();

public slots:
    void openTsv();

};

#endif // MAINWINDOW_H
