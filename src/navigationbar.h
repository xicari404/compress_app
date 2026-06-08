#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

class NavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBar(QWidget *parent = nullptr);
    void setActiveStep(int step);
    void setStepEnabled(int step, bool enabled);

signals:
    void stepClicked(int step);

private:
    void createStepButtons();
    void setupStyles();
    void onStepButtonClicked(int step);

    QPushButton *stepButtons[5];
    QLabel *stepLabels[5];
    QString stepNames[5] = {
        "Analyse",
        "Résultats",
        "Sélection",
        "Compression",
        "Terminé"
    };
    int activeStep;
};

#endif // NAVIGATIONBAR_H
