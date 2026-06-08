#ifndef STEP4_COMPRESSION_H
#define STEP4_COMPRESSION_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>

class Step4Compression : public QWidget
{
    Q_OBJECT

public:
    explicit Step4Compression(QWidget *parent = nullptr);

signals:
    void compressionFinished();

private slots:
    void simulateCompression();
    void updateProgress();

private:
    void setupUI();
    void setupStyles();

    QProgressBar *overallProgress;
    QProgressBar *fileProgress;
    QLabel *statusLabel;
    QLabel *detailsLabel;
    QPushButton *pauseButton;
    int compressionValue;
};

#endif // STEP4_COMPRESSION_H
