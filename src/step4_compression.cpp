#include "step4_compression.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

Step4Compression::Step4Compression(QWidget *parent)
    : QWidget(parent), compressionValue(0)
{
    setupUI();
    setupStyles();
    
    // Start simulation when widget is shown
    QTimer::singleShot(500, this, &Step4Compression::simulateCompression);
}

void Step4Compression::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape 4: Compression en cours", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);
    
    // Status
    statusLabel = new QLabel("Initialisation de la compression...", this);
    statusLabel->setStyleSheet("color: #666; font-size: 14px;");
    mainLayout->addWidget(statusLabel);
    
    // Overall progress
    QLabel *overallLabel = new QLabel("Progression globale:", this);
    overallLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
    mainLayout->addWidget(overallLabel);
    
    overallProgress = new QProgressBar(this);
    overallProgress->setMinimum(0);
    overallProgress->setMaximum(100);
    overallProgress->setValue(0);
    mainLayout->addWidget(overallProgress);
    
    // File progress
    QLabel *fileLabel = new QLabel("Fichier en cours:", this);
    fileLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
    mainLayout->addWidget(fileLabel);
    
    fileProgress = new QProgressBar(this);
    fileProgress->setMinimum(0);
    fileProgress->setMaximum(100);
    fileProgress->setValue(0);
    mainLayout->addWidget(fileProgress);
    
    // Details
    detailsLabel = new QLabel(this);
    detailsLabel->setStyleSheet("color: #555; background-color: #f5f5f5; padding: 15px; border-radius: 5px; font-size: 13px;");
    detailsLabel->setWordWrap(true);
    detailsLabel->setText(
        "Fichiers traités: 0 / 8,432\n"
        "Taille compressée: 0 GB / 125 GB\n"
        "Temps écoulé: 0:00\n"
        "Temps estimé: 2h 30m"
    );
    mainLayout->addWidget(detailsLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    pauseButton = new QPushButton("Pause", this);
    pauseButton->setMinimumWidth(150);
    pauseButton->setMinimumHeight(40);
    buttonLayout->addWidget(pauseButton);
    
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void Step4Compression::setupStyles()
{
    QString buttonStyle = R"(
        QPushButton {
            background-color: #ffc107;
            color: #333;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #e0a800;
        }
    )";
    
    pauseButton->setStyleSheet(buttonStyle);
}

void Step4Compression::simulateCompression()
{
    QTimer *timer = new QTimer(this);
    
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        updateProgress();
        
        if (compressionValue >= 100) {
            timer->stop();
            statusLabel->setText("✓ Compression terminée!");
            emit compressionFinished();
        }
    });
    
    timer->start(100); // Update every 100ms
}

void Step4Compression::updateProgress()
{
    compressionValue++;
    
    overallProgress->setValue(compressionValue);
    
    // Simulate file progress
    int fileProgress = (compressionValue * 3) % 100;
    this->fileProgress->setValue(fileProgress);
    
    // Update details
    int filesProcessed = (compressionValue * 84) / 100;
    double compressedSize = compressionValue * 1.25;
    int minutes = (compressionValue / 2);
    int seconds = (compressionValue * 3) % 60;
    
    detailsLabel->setText(QString(
        "Fichiers traités: %1 / 8,432\n"
        "Taille compressée: %2 GB / 125 GB\n"
        "Temps écoulé: 2:%3\n"
        "Temps estimé: 2h 30m"
    ).arg(filesProcessed)
     .arg(compressedSize, 0, 'f', 1)
     .arg(seconds, 2, 10, QChar('0')));
    
    statusLabel->setText(QString("Compression en cours... %1%").arg(compressionValue));
}
