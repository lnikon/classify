#ifndef CLASSIFICATIONDIALOG_H
#define CLASSIFICATIONDIALOG_H

#include <QWidget>

class QComboBox;
class QPushButton;
class QLabel;
class QVBoxLayout;
class QAbstractItemModel;

class ClassificationDialog : public QWidget
{
    Q_OBJECT
public:
    explicit ClassificationDialog(QWidget *parent = nullptr);

    void setModel(const QAbstractItemModel* pChartView);

signals:

public slots:
    void setIndex(const int index);
    void classifyButtonClicked();

private:
    QComboBox* m_pComboBox{nullptr};
    QPushButton* m_pButton{nullptr};
    QLabel* m_pLabel{nullptr};
    QVBoxLayout* m_pMainLayout{nullptr};
    const QAbstractItemModel* m_pModel{nullptr};

    int m_currentIndex{0};

};

#endif // CLASSIFICATIONDIALOG_H
