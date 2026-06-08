#include "step5_results.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

Step5Results::Step5Results(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
}

void Step5Results::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape 5: Compression terminée!", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #28a745;");
    mainLayout->addWidget(titleLabel);
    
    // Success message
    QLabel *messageLabel = new QLabel(
        "🎉 Votre disque a été optimisé avec succès!",
        this
    );
    messageLabel->setStyleSheet("color: #666; font-size: 16px; margin: 20px 0;");
    mainLayout->addWidget(messageLabel);
    
    // Results box
    QLabel *resultsBox = new QLabel(this);
    resultsBox->setStyleSheet(
        "background-color: #f5f5f5; padding: 20px; border-radius: 5px; "
        "border-left: 4px solid #28a745;"
    );
    resultsBox->setWordWrap(true);
    resultsBox->setText(
        "📊 Résumé de l'optimisation:\n\n"
        "✓ Espace total récupéré: 125 GB\n"
        "✓ Fichiers compressés: 8,432\n"
        "✓ Ratio de compression: 45%\n"
        "✓ Temps de compression: 2h 34m\n\n"
        "Avant: 475 GB utilisés (95%)\n"
        "Après: 350 GB utilisés (70%)\n"
        "Espace libre maintenant: 150 GB"
    );
    mainLayout->addWidget(resultsBox);
    
    // Additional info
    QLabel *infoLabel = new QLabel(
        "💡 Conseils:\n"
        "• Les fichiers compressés sont transparents à l'utilisation\n"
        "• Vous pouvez continuer à utiliser vos fichiers normalement\n"
        "• L'accès sera légèrement plus lent (décompression à la demande)\n"
        "• Vérifiez votre disque pour les résultats réels",
        this
    );
    infoLabel->setStyleSheet("color: #555; font-size: 13px; background-color: #e7f3ff; padding: 15px; border-radius: 5px;");
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *viewDetailsButton = new QPushButton("Voir les détails", this);
    viewDetailsButton->setMinimumWidth(150);
    viewDetailsButton->setMinimumHeight(40);
    buttonLayout->addWidget(viewDetailsButton);
    
    finishButton = new QPushButton("Terminer", this);
    finishButton->setMinimumWidth(150);
    finishButton->setMinimumHeight(40);
    connect(finishButton, &QPushButton::clicked, this, &Step5Results::onFinishClicked);
    buttonLayout->addWidget(finishButton);
    
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
}

void Step5Results::setupStyles()
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
    
    finishButton->setStyleSheet(buttonStyle);
}

void Step5Results::onFinishClicked()
{
    // Could emit signal to main window or close app
    qApp->quit();
}
