#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

enum class ActionIndex
{
    OPEN_TSV = 0,
    SAVE,
    KNN,
    CLASSIFY,
    EXIT,
    ENUM_SIZE
};

class CSVTableView;
class QAction;
class QProgressBar;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow      *ui{nullptr};
    CSVTableView        *csvTableView_{nullptr};
    QProgressBar        *progressBar_{nullptr};
    QVector<QAction *>  actions_;

    void setupTop();

public slots:
    void openTsv();
    void classify();

};

#endif // MAINWINDOW_H
