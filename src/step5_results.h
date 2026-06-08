#ifndef STEP5_RESULTS_H
#define STEP5_RESULTS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class Step5Results : public QWidget
{
    Q_OBJECT

public:
    explicit Step5Results(QWidget *parent = nullptr);

private slots:
    void onFinishClicked();

private:
    void setupUI();
    void setupStyles();

    QPushButton *finishButton;
};

#endif // STEP5_RESULTS_H
