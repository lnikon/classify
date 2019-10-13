#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdsvtablemodel.h"
#include "csvtablemodel.h"
#include "csvtableview.h"

#include <ctime>

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    csvTableView_{new CSVTableView(parent)}
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(csvTableView_);

    setupTop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTop()
{
    QMenu* file = menuBar()->addMenu("&File");

    QAction* openAction = new QAction(QIcon(":/icons/import_csv.png"), "Open CSV/TSV", this);
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
        CsvTableModel *model = new CsvTableModel(this);
        QString extension = QFileInfo(QFile(fileName)).completeSuffix();
        if (extension.toLower() == "csv" || extension.toLower() == "tsv") //known file extensions
        {
            auto code = model->loadFromFile(fileName);
            if(code != MemoryMappedCsvContainer::ReturnCode::SUCCESS)
            {
                QString errorCodeString = (code == MemoryMappedCsvContainer::ReturnCode::INVALID_DESCRIPTOR ? "INVALID_DESCRIPTOR" : "INVALID_MMAP_ADDRESS");
                QMessageBox::critical(this, QString("Unable to open file"), QString("Can't open file\nError code: " + errorCodeString));
                return;
            }

            csvTableView_->setModel(model);
        }
    }
}
