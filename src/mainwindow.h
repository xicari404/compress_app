#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QProgressBar>
#include <QLabel>
#include "navigationbar.h"
#include "step1_analysis.h"
#include "step2_results.h"
#include "step3_selection.h"
#include "step4_compression.h"
#include "step5_results.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNavigationClicked(int step);
    void onStepCompleted(int currentStep);
    void onCompressionFinished();

private:
    void setupUI();
    void createNavigationBar();
    void createContentArea();
    void connectSignals();

    // UI Components
    NavigationBar *navBar;
    QStackedWidget *contentStack;
    
    // Step widgets
    Step1Analysis *step1;
    Step2Results *step2;
    Step3Selection *step3;
    Step4Compression *step4;
    Step5Results *step5;
    
    // Current step
    int currentStep;
};

#endif // MAINWINDOW_H
