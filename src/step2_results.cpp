#include "step2_results.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

Step2Results::Step2Results(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
}

void Step2Results::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape 2: Résultats d'analyse", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);
    
    // Summary
    summaryLabel = new QLabel(this);
    summaryLabel->setStyleSheet("color: #666; font-size: 14px; background-color: #f5f5f5; padding: 15px; border-radius: 5px;");
    summaryLabel->setWordWrap(true);
    summaryLabel->setText(
        "Espace récupérable: 125 GB\n"
        "Fichiers analysés: 8,432\n"
        "Ratio de compression moyen: 45%"
    );
    mainLayout->addWidget(summaryLabel);
    
    // Results table
    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(5);
    resultsTable->setHorizontalHeaderLabels({"Type", "Fichiers", "Taille", "Compressible", "Économies"});
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Mock data
    resultsTable->insertRow(0);
    resultsTable->setItem(0, 0, new QTableWidgetItem("Vidéos"));
    resultsTable->setItem(0, 1, new QTableWidgetItem("45"));
    resultsTable->setItem(0, 2, new QTableWidgetItem("150 GB"));
    resultsTable->setItem(0, 3, new QTableWidgetItem("Oui"));
    resultsTable->setItem(0, 4, new QTableWidgetItem("60 GB"));
    
    resultsTable->insertRow(1);
    resultsTable->setItem(1, 0, new QTableWidgetItem("Photos"));
    resultsTable->setItem(1, 1, new QTableWidgetItem("2,150"));
    resultsTable->setItem(1, 2, new QTableWidgetItem("80 GB"));
    resultsTable->setItem(1, 3, new QTableWidgetItem("Oui"));
    resultsTable->setItem(1, 4, new QTableWidgetItem("20 GB"));
    
    resultsTable->insertRow(2);
    resultsTable->setItem(2, 0, new QTableWidgetItem("Documents"));
    resultsTable->setItem(2, 1, new QTableWidgetItem("1,500"));
    resultsTable->setItem(2, 2, new QTableWidgetItem("5 GB"));
    resultsTable->setItem(2, 3, new QTableWidgetItem("Oui"));
    resultsTable->setItem(2, 4, new QTableWidgetItem("4 GB"));
    
    mainLayout->addWidget(resultsTable);
    
    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *backButton = new QPushButton("← Retour", this);
    backButton->setMinimumWidth(150);
    backButton->setMinimumHeight(40);
    buttonLayout->addWidget(backButton);
    
    nextButton = new QPushButton("Suivant →", this);
    nextButton->setMinimumWidth(150);
    nextButton->setMinimumHeight(40);
    connect(nextButton, &QPushButton::clicked, this, &Step2Results::onNextButtonClicked);
    buttonLayout->addWidget(nextButton);
    
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void Step2Results::setupStyles()
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
    )";
    
    nextButton->setStyleSheet(buttonStyle);
    
    resultsTable->setStyleSheet(R"(
        QTableWidget {
            border: 1px solid #bbb;
            border-radius: 5px;
        }
        QTableWidget::item {
            padding: 5px;
        }
        QHeaderView::section {
            background-color: #007BFF;
            color: white;
            padding: 5px;
            border: none;
        }
    )");
}

void Step2Results::setupResultsTable()
{
    // This would be populated from analysis results
}

void Step2Results::onNextButtonClicked()
{
    emit nextStepClicked();
}
