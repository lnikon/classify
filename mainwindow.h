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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QAction *> actions_;

    void setupTop();

public slots:
    void openTsv();

};

#endif // MAINWINDOW_H
