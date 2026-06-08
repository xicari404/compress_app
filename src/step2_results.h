#ifndef STEP2_RESULTS_H
#define STEP2_RESULTS_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

class Step2Results : public QWidget
{
    Q_OBJECT

public:
    explicit Step2Results(QWidget *parent = nullptr);

signals:
    void nextStepClicked();

private slots:
    void setupResultsTable();
    void onNextButtonClicked();

private:
    void setupUI();
    void setupStyles();

    QTableWidget *resultsTable;
    QLabel *summaryLabel;
    QPushButton *nextButton;
};

#endif // STEP2_RESULTS_H
