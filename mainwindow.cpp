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
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    csvTableView_{new CSVTableView(parent)},
    progressBar_{new QProgressBar(parent)}
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
    connect(actions_[static_cast<int>(ActionIndex::OPEN_TSV)], &QAction::triggered, this, &MainWindow::openTsv);

    actions_.push_back(new QAction());
    actions_.push_back(new QAction());

    QAction* classifyAction = new QAction(QIcon(":/icons/mindmap.png"), "Classify", this);
    actions_.push_back(classifyAction);
    file->addAction(classifyAction);
    ui->mainToolBar->addAction(classifyAction);
    connect(actions_[static_cast<int>(ActionIndex::CLASSIFY)], &QAction::triggered, this, &MainWindow::classify);

    ui->mainToolBar->addSeparator();

    QAction* exitAction = new QAction(QIcon(":/icons/exit.png"), "Exit", this);
    actions_.push_back(exitAction);
    file->addAction(exitAction);
    ui->mainToolBar->addAction(exitAction);
    connect(actions_[static_cast<int>(ActionIndex::EXIT)], QOverload<bool>::of(&QAction::triggered), this,[=]()
    {
        qApp->exit();
    });
}

void MainWindow::openTsv()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save Delimiter-Separated Value"),
                                                    QDir::currentPath(), "All files (*.*);;"
                                                                         "CSV (*.csv);; TSV (*.tsv)");
    progressBar_->show();
    if (!fileName.isEmpty())
    {
        CsvTableModel *model = new CsvTableModel(this);
        QString extension = QFileInfo(QFile(fileName)).completeSuffix();
        if (extension.toLower() == "csv" || extension.toLower() == "tsv") //known file extensions
        {
            auto code = model->loadFromFile(fileName, '\t');
            if(code != MemoryMappedCsvContainer::ReturnCode::SUCCESS)
            {
                QString errorCodeString = (code == MemoryMappedCsvContainer::ReturnCode::INVALID_DESCRIPTOR ? "INVALID_DESCRIPTOR" : "INVALID_MMAP_ADDRESS");
                QMessageBox::critical(this, QString("Unable to open file"), QString("Can't open file\nError code: " + errorCodeString));
                return;
            }

            csvTableView_->setModel(model);
        }
    }
    progressBar_->hide();
}

void MainWindow::classify()
{
    csvTableView_->onUniqueCountAction();
}
