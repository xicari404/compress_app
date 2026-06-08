#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentStep(0)
{
    setupUI();
    connectSignals();
    
    // Set window size
    resize(1000, 700);
    
    // Center on screen
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create navigation bar
    navBar = new NavigationBar(this);
    mainLayout->addWidget(navBar);
    
    // Create stacked widget for content
    contentStack = new QStackedWidget(this);
    mainLayout->addWidget(contentStack, 1);
    
    // Create step widgets
    step1 = new Step1Analysis(this);
    step2 = new Step2Results(this);
    step3 = new Step3Selection(this);
    step4 = new Step4Compression(this);
    step5 = new Step5Results(this);
    
    // Add to stack
    contentStack->addWidget(step1);  // Index 0
    contentStack->addWidget(step2);  // Index 1
    contentStack->addWidget(step3);  // Index 2
    contentStack->addWidget(step4);  // Index 3
    contentStack->addWidget(step5);  // Index 4
    
    // Show first step
    contentStack->setCurrentIndex(0);
}

void MainWindow::connectSignals()
{
    // Connect navigation bar to step changes
    connect(navBar, &NavigationBar::stepClicked, this, &MainWindow::onNavigationClicked);
    
    // Connect step signals
    connect(step1, &Step1Analysis::analysisCompleted, this, [this]() {
        currentStep = 1;
        contentStack->setCurrentIndex(1);
        navBar->setActiveStep(1);
    });
    
    connect(step2, &Step2Results::nextStepClicked, this, [this]() {
        currentStep = 2;
        contentStack->setCurrentIndex(2);
        navBar->setActiveStep(2);
    });
    
    connect(step3, &Step3Selection::selectionDone, this, [this]() {
        currentStep = 3;
        contentStack->setCurrentIndex(3);
        navBar->setActiveStep(3);
    });
    
    connect(step4, &Step4Compression::compressionFinished, this, [this]() {
        currentStep = 4;
        contentStack->setCurrentIndex(4);
        navBar->setActiveStep(4);
    });
}

void MainWindow::onNavigationClicked(int step)
{
    if (step >= 0 && step <= 4) {
        currentStep = step;
        contentStack->setCurrentIndex(step);
        navBar->setActiveStep(step);
    }
}

void MainWindow::onStepCompleted(int currentStep)
{
    // Handle step completion
    if (currentStep < 4) {
        onNavigationClicked(currentStep + 1);
    }
}

void MainWindow::onCompressionFinished()
{
    // Update status when compression is complete
    navBar->setActiveStep(4);
}
