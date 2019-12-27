#include "classificationdialog.h"
#include "chartwidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QAbstractItemModel>

ClassificationDialog::ClassificationDialog(QWidget *parent) : QWidget(parent)
{
    m_pComboBox = new QComboBox;
    m_pButton = new QPushButton("Classify");
    m_pLabel = new QLabel("Choose category for classification");
    m_pMainLayout = new QVBoxLayout(this);

    m_pMainLayout->addWidget(m_pLabel);
    m_pMainLayout->addWidget(m_pComboBox);
    m_pMainLayout->addWidget(m_pButton);

    setWindowModality(Qt::WindowModality::ApplicationModal);

    setLayout(m_pMainLayout);
    setWindowTitle(tr("Classification Wizard"));

    connect(m_pComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, QOverload<int>::of(&ClassificationDialog::setIndex));

    connect(m_pButton, QOverload<bool>::of(&QPushButton::clicked), this, QOverload<>::of(&ClassificationDialog::classifyButtonClicked));
}

void ClassificationDialog::setModel(const QAbstractItemModel* pModel)
{
    m_pModel = pModel;

    const auto colCnt = m_pModel->columnCount();
    QStringList categories;
    for (int i = 0; i < colCnt; ++i)
    {
        categories.push_back(m_pModel->headerData(i, Qt::Horizontal).toString());
    }

    m_pComboBox->addItems(categories);
}

void ClassificationDialog::setIndex(const int index)
{
    m_currentIndex = index;
}

void ClassificationDialog::classifyButtonClicked()
{
    ChartWidget* pChartWgt = new ChartWidget;
    pChartWgt->setPModel(m_pModel);
    pChartWgt->prepare(m_currentIndex);
    pChartWgt->show();
}
