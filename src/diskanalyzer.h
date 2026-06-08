#ifndef DISKANALYZER_H
#define DISKANALYZER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QObject>

struct FileInfo {
    QString path;
    QString name;
    qint64 size;
    QString type; // "video", "photo", "document", "archive", "other"
    bool isDuplicate;
    QString duplicateOf;
    QDateTime lastAccessed;
};

struct DiskAnalysisResult {
    QString driveLetter;
    qint64 totalSize;
    qint64 usedSize;
    qint64 freeSize;
    double usagePercent;
    QVector<FileInfo> files;
    qint64 recoverableSpace;
};

class DiskAnalyzer : public QObject
{
    Q_OBJECT

public:
    explicit DiskAnalyzer(QObject *parent = nullptr);
    void analyzeDisk(const QString &drivePath);
    DiskAnalysisResult getResults() const { return analysisResult; }

signals:
    void analysisProgress(int percent);
    void analysisFinished();
    void analysisError(const QString &error);

private:
    void analyzeDirectory(const QString &path, QVector<FileInfo> &files);
    QString getFileType(const QString &filePath);
    qint64 getDirectorySize(const QString &path);
    void findDuplicates(QVector<FileInfo> &files);
    qint64 calculateRecoverableSpace();

    DiskAnalysisResult analysisResult;
    QMap<QString, FileInfo> fileHashes; // For duplicate detection
};

#endif // DISKANALYZER_H
