#ifndef STEP1_ANALYSIS_H
#define STEP1_ANALYSIS_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "diskanalyzer.h"

class Step1Analysis : public QWidget
{
    Q_OBJECT

public:
    explicit Step1Analysis(QWidget *parent = nullptr);

signals:
    void analysisCompleted();

private slots:
    void onAnalyzeButtonClicked();
    void onAnalysisProgress(int percent);
    void onAnalysisFinished();
    void onAnalysisError(const QString &error);
    void onDriveSelected(const QString &drive);

private:
    void setupUI();
    void setupStyles();
    QStringList getDriveLetters();

    QComboBox *driveCombo;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QLabel *detailsLabel;
    QPushButton *analyzeButton;
    QPushButton *nextButton;
    DiskAnalyzer *analyzer;
    QString selectedDrive;
};

#endif // STEP1_ANALYSIS_H
