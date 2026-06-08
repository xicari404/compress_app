#ifndef STEP3_SELECTION_H
#define STEP3_SELECTION_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>

class Step3Selection : public QWidget
{
    Q_OBJECT

public:
    explicit Step3Selection(QWidget *parent = nullptr);

signals:
    void selectionDone();

private slots:
    void onSelectionChanged();
    void onStartCompression();

private:
    void setupUI();
    void setupStyles();

    QMap<QString, QCheckBox*> fileTypeCheckboxes;
    QLabel *summaryLabel;
    QPushButton *compressionButton;
};

#endif // STEP3_SELECTION_H
