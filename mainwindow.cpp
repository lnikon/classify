#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdsvtablemodel.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTop()
{
    QMenu* file = menuBar()->addMenu("&File");

    QAction* openAction = new QAction(QIcon(":/icons/open.png"), "Open TSV", this);
    actions_.push_back(openAction);
    file->addAction(openAction);
    ui->mainToolBar->addAction(openAction);
    connect(actions_[OPEN_TSV], &QAction::triggered, this, &MainWindow::openTsv);
}

void MainWindow::openTsv()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save Delimiter-Separated Value"),
                                                    QDir::currentPath(), "All files (*.*);;"
                                                    "CSV (*.csv);; TSV (*.tsv)");
    if (!fileName.isEmpty()) {
        QDsvTableModel *model = new QDsvTableModel(this);
        QString extension = QFileInfo(QFile(fileName)).completeSuffix();
        if (extension.toLower() == "csv" || extension.toLower() == "tsv") //known file extensions
            model->loadFromFile(fileName);
        else {
            while (true) {
                bool ok;
                QString s = QInputDialog::getText(this, tr("Unknown File Format"),
                                                  tr("Enter Delimiter:"), QLineEdit::Normal,
                                                  "", &ok);
                if (ok && s.size() == 1) {
                    QChar delim = s.constData()[0];
                    model->loadFromFile(fileName, delim);
                    break;
                }
            }
        }
        ui->tableView->setModel(model);
    }
}
