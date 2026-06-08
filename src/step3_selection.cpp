#include "step3_selection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

Step3Selection::Step3Selection(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
}

void Step3Selection::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape 3: Sélection des fichiers", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);
    
    // Description
    QLabel *descLabel = new QLabel(
        "Sélectionnez les types de fichiers que vous souhaitez compresser:",
        this
    );
    descLabel->setStyleSheet("color: #666; font-size: 14px;");
    mainLayout->addWidget(descLabel);
    
    // Selection group
    QGroupBox *selectionGroup = new QGroupBox("Types de fichiers", this);
    QVBoxLayout *groupLayout = new QVBoxLayout(selectionGroup);
    groupLayout->setSpacing(10);
    
    // Video files
    QCheckBox *videoCheckbox = new QCheckBox("📹 Vidéos (150 GB) → Économise: 60 GB", this);
    videoCheckbox->setChecked(true);
    groupLayout->addWidget(videoCheckbox);
    fileTypeCheckboxes["video"] = videoCheckbox;
    
    // Photos
    QCheckBox *photoCheckbox = new QCheckBox("📷 Photos (80 GB) → Économise: 20 GB", this);
    photoCheckbox->setChecked(true);
    groupLayout->addWidget(photoCheckbox);
    fileTypeCheckboxes["photo"] = photoCheckbox;
    
    // Documents
    QCheckBox *docCheckbox = new QCheckBox("📄 Documents (5 GB) → Économise: 4 GB", this);
    docCheckbox->setChecked(true);
    groupLayout->addWidget(docCheckbox);
    fileTypeCheckboxes["document"] = docCheckbox;
    
    // Temp files
    QCheckBox *tempCheckbox = new QCheckBox("🗑️ Fichiers temporaires (30 GB) → Supprime: 30 GB", this);
    tempCheckbox->setChecked(true);
    groupLayout->addWidget(tempCheckbox);
    fileTypeCheckboxes["temp"] = tempCheckbox;
    
    // Duplicates
    QCheckBox *dupCheckbox = new QCheckBox("🔀 Doublons (15 GB) → Supprime: 15 GB", this);
    dupCheckbox->setChecked(true);
    groupLayout->addWidget(dupCheckbox);
    fileTypeCheckboxes["duplicates"] = dupCheckbox;
    
    mainLayout->addWidget(selectionGroup);
    
    // Connect checkboxes
    for (auto checkbox : fileTypeCheckboxes) {
        connect(checkbox, &QCheckBox::stateChanged, this, &Step3Selection::onSelectionChanged);
    }
    
    // Summary
    summaryLabel = new QLabel(this);
    summaryLabel->setStyleSheet("color: #333; font-size: 14px; background-color: #f5f5f5; padding: 15px; border-radius: 5px;");
    summaryLabel->setWordWrap(true);
    summaryLabel->setText("Espace économisable: 129 GB");
    mainLayout->addWidget(summaryLabel);
    
    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *backButton = new QPushButton("← Retour", this);
    backButton->setMinimumWidth(150);
    backButton->setMinimumHeight(40);
    buttonLayout->addWidget(backButton);
    
    compressionButton = new QPushButton("Démarrer la compression →", this);
    compressionButton->setMinimumWidth(200);
    compressionButton->setMinimumHeight(40);
    connect(compressionButton, &QPushButton::clicked, this, &Step3Selection::onStartCompression);
    buttonLayout->addWidget(compressionButton);
    
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void Step3Selection::setupStyles()
{
    QString buttonStyle = R"(
        QPushButton {
            background-color: #28a745;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #218838;
        }
        QPushButton:pressed {
            background-color: #1e7e34;
        }
    )";
    
    compressionButton->setStyleSheet(buttonStyle);
}

void Step3Selection::onSelectionChanged()
{
    int totalEconomies = 0;
    
    if (fileTypeCheckboxes["video"]->isChecked()) totalEconomies += 60;
    if (fileTypeCheckboxes["photo"]->isChecked()) totalEconomies += 20;
    if (fileTypeCheckboxes["document"]->isChecked()) totalEconomies += 4;
    if (fileTypeCheckboxes["temp"]->isChecked()) totalEconomies += 30;
    if (fileTypeCheckboxes["duplicates"]->isChecked()) totalEconomies += 15;
    
    summaryLabel->setText(QString("Espace économisable: %1 GB").arg(totalEconomies));
}

void Step3Selection::onStartCompression()
{
    emit selectionDone();
}
