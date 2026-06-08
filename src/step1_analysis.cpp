#include "step1_analysis.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStorageInfo>
#include <QTimer>

Step1Analysis::Step1Analysis(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
    
    analyzer = new DiskAnalyzer(this);
    connect(analyzer, &DiskAnalyzer::analysisProgress, this, &Step1Analysis::onAnalysisProgress);
    connect(analyzer, &DiskAnalyzer::analysisFinished, this, &Step1Analysis::onAnalysisFinished);
    connect(analyzer, &DiskAnalyzer::analysisError, this, &Step1Analysis::onAnalysisError);
}

void Step1Analysis::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape 1: Analyse du disque", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);
    
    // Description
    QLabel *descLabel = new QLabel(
        "Sélectionnez le disque à analyser. L'application analysera tous les fichiers "
        "pour identifier les opportunités de compression et d'optimisation.",
        this
    );
    descLabel->setStyleSheet("color: #666; font-size: 14px; line-height: 1.5;");
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);
    
    // Drive selection
    QHBoxLayout *driveLayout = new QHBoxLayout();
    driveLayout->setSpacing(10);
    
    QLabel *driveLabel = new QLabel("Sélectionnez un disque:", this);
    driveLabel->setStyleSheet("font-weight: bold;");
    driveLayout->addWidget(driveLabel);
    
    driveCombo = new QComboBox(this);
    driveCombo->setMinimumWidth(200);
    
    QStringList drives = getDriveLetters();
    for (const QString &drive : drives) {
        driveCombo->addItem(drive);
    }
    
    connect(driveCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &Step1Analysis::onDriveSelected);
    
    driveLayout->addWidget(driveCombo);
    driveLayout->addStretch();
    mainLayout->addLayout(driveLayout);
    
    // Disk info
    QLabel *infoLabel = new QLabel("Informations du disque:", this);
    infoLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(infoLabel);
    
    detailsLabel = new QLabel(this);
    detailsLabel->setStyleSheet("color: #555; background-color: #f5f5f5; padding: 15px; border-radius: 5px;");
    detailsLabel->setWordWrap(true);
    mainLayout->addWidget(detailsLabel);
    
    onDriveSelected(driveCombo->currentText());
    
    // Progress section
    statusLabel = new QLabel("Prêt à analyser", this);
    statusLabel->setStyleSheet("color: #666; font-size: 13px;");
    mainLayout->addWidget(statusLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);
    
    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    analyzeButton = new QPushButton("Analyser le disque", this);
    analyzeButton->setMinimumWidth(150);
    analyzeButton->setMinimumHeight(40);
    connect(analyzeButton, &QPushButton::clicked, this, &Step1Analysis::onAnalyzeButtonClicked);
    buttonLayout->addWidget(analyzeButton);
    
    nextButton = new QPushButton("Suivant →", this);
    nextButton->setMinimumWidth(150);
    nextButton->setMinimumHeight(40);
    nextButton->setEnabled(false);
    connect(nextButton, &QPushButton::clicked, this, &Step1Analysis::analysisCompleted);
    buttonLayout->addWidget(nextButton);
    
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void Step1Analysis::setupStyles()
{
    QString buttonStyle = R"(
        QPushButton {
            background-color: #007BFF;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #0056b3;
        }
        QPushButton:pressed {
            background-color: #003d82;
        }
        QPushButton:disabled {
            background-color: #ccc;
            color: #666;
        }
    )";
    
    analyzeButton->setStyleSheet(buttonStyle);
    nextButton->setStyleSheet(buttonStyle);
    
    driveCombo->setStyleSheet(R"(
        QComboBox {
            border: 1px solid #bbb;
            border-radius: 5px;
            padding: 5px;
            background-color: white;
        }
    )");
}

QStringList Step1Analysis::getDriveLetters()
{
    QStringList drives;
    
#ifdef Q_OS_WIN
    for (char drive = 'A'; drive <= 'Z'; drive++) {
        QString drivePath = QString(drive) + ":/";
        QStorageInfo storage(drivePath);
        if (storage.isValid() && storage.bytesTotal() > 0) {
            drives.append(drivePath);
        }
    }
#else
    drives.append("/home");
    drives.append("/");
#endif
    
    return drives;
}

void Step1Analysis::onAnalyzeButtonClicked()
{
    if (selectedDrive.isEmpty()) {
        statusLabel->setText("Veuillez sélectionner un disque");
        return;
    }
    
    analyzeButton->setEnabled(false);
    progressBar->setVisible(true);
    progressBar->setValue(0);
    statusLabel->setText("Analyse en cours...");
    
    analyzer->analyzeDisk(selectedDrive);
}

void Step1Analysis::onAnalysisProgress(int percent)
{
    progressBar->setValue(percent);
    statusLabel->setText(QString("Analyse en cours... %1%").arg(percent));
}

void Step1Analysis::onAnalysisFinished()
{
    progressBar->setVisible(false);
    statusLabel->setText("Analyse terminée ✓");
    analyzeButton->setEnabled(true);
    nextButton->setEnabled(true);
    progressBar->setValue(100);
}

void Step1Analysis::onAnalysisError(const QString &error)
{
    progressBar->setVisible(false);
    statusLabel->setText("Erreur: " + error);
    analyzeButton->setEnabled(true);
    nextButton->setEnabled(false);
}

void Step1Analysis::onDriveSelected(const QString &drive)
{
    selectedDrive = drive;
    
    QStorageInfo storage(drive);
    qint64 totalSize = storage.bytesTotal();
    qint64 usedSize = storage.bytesTotal() - storage.bytesFree();
    qint64 freeSize = storage.bytesFree();
    double usagePercent = (double)usedSize / totalSize * 100;
    
    QString details = QString(
        "Disque: %1\n"
        "Taille totale: %2 GB\n"
        "Utilisé: %3 GB (%4%)\n"
        "Libre: %5 GB"
    ).arg(drive)
     .arg(totalSize / (1024 * 1024 * 1024))
     .arg(usedSize / (1024 * 1024 * 1024))
     .arg(usagePercent, 0, 'f', 1)
     .arg(freeSize / (1024 * 1024 * 1024));
    
    detailsLabel->setText(details);
}
