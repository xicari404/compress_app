#include "navigationbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleFactory>
#include <QStyle>

NavigationBar::NavigationBar(QWidget *parent)
    : QWidget(parent), activeStep(0)
{
    setStyleSheet("background-color: #f0f0f0; border-bottom: 1px solid #ddd;");
    setMaximumHeight(120);
    setMinimumHeight(120);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(10);
    
    // Title
    QLabel *titleLabel = new QLabel("Étape de compression", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);
    
    // Navigation buttons layout
    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->setSpacing(10);
    
    createStepButtons();
    
    for (int i = 0; i < 5; i++) {
        navLayout->addWidget(stepButtons[i]);
        
        // Add separator between buttons
        if (i < 4) {
            QLabel *separator = new QLabel("→", this);
            separator->setStyleSheet("color: #999; font-size: 14px;");
            navLayout->addWidget(separator, 0, Qt::AlignCenter);
        }
    }
    
    navLayout->addStretch();
    mainLayout->addLayout(navLayout);
    
    setupStyles();
}

void NavigationBar::createStepButtons()
{
    for (int i = 0; i < 5; i++) {
        stepButtons[i] = new QPushButton(stepNames[i], this);
        stepButtons[i]->setMinimumWidth(100);
        stepButtons[i]->setMaximumWidth(120);
        stepButtons[i]->setProperty("step", i);
        stepButtons[i]->setCursor(Qt::PointingHandCursor);
        
        // Connect button click
        connect(stepButtons[i], &QPushButton::clicked, this, [this, i]() {
            onStepButtonClicked(i);
        });
    }
}

void NavigationBar::setupStyles()
{
    QString activeStyle = R"(
        QPushButton {
            background-color: #007BFF;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px 12px;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #0056b3;
        }
    )";
    
    QString inactiveStyle = R"(
        QPushButton {
            background-color: #e0e0e0;
            color: #666;
            border: 1px solid #bbb;
            border-radius: 5px;
            padding: 8px 12px;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #d0d0d0;
        }
    )";
    
    QString disabledStyle = R"(
        QPushButton {
            background-color: #f0f0f0;
            color: #999;
            border: 1px solid #ddd;
            border-radius: 5px;
            padding: 8px 12px;
            font-size: 12px;
        }
    )";
    
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            stepButtons[i]->setStyleSheet(activeStyle);
        } else if (i <= 1) {
            stepButtons[i]->setStyleSheet(inactiveStyle);
        } else {
            stepButtons[i]->setStyleSheet(disabledStyle);
            stepButtons[i]->setEnabled(false);
        }
    }
}

void NavigationBar::setActiveStep(int step)
{
    if (step >= 0 && step < 5) {
        activeStep = step;
        
        QString activeStyle = R"(
            QPushButton {
                background-color: #007BFF;
                color: white;
                border: none;
                border-radius: 5px;
                padding: 8px 12px;
                font-weight: bold;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #0056b3;
            }
        )";
        
        QString inactiveStyle = R"(
            QPushButton {
                background-color: #e0e0e0;
                color: #666;
                border: 1px solid #bbb;
                border-radius: 5px;
                padding: 8px 12px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #d0d0d0;
            }
        )";
        
        for (int i = 0; i < 5; i++) {
            if (i == step) {
                stepButtons[i]->setStyleSheet(activeStyle);
                stepButtons[i]->setEnabled(true);
            } else if (i < step) {
                stepButtons[i]->setStyleSheet(inactiveStyle);
                stepButtons[i]->setEnabled(true);
            } else {
                stepButtons[i]->setStyleSheet(inactiveStyle);
                stepButtons[i]->setEnabled(false);
            }
        }
    }
}

void NavigationBar::setStepEnabled(int step, bool enabled)
{
    if (step >= 0 && step < 5) {
        stepButtons[step]->setEnabled(enabled);
    }
}

void NavigationBar::onStepButtonClicked(int step)
{
    if (step <= activeStep) {
        setActiveStep(step);
        emit stepClicked(step);
    }
}
