#ifndef COMPRESSIONENGINE_H
#define COMPRESSIONENGINE_H

#include <QString>
#include <QVector>
#include <QObject>
#include <zstd.h>

struct CompressionStats {
    QString filePath;
    qint64 originalSize;
    qint64 compressedSize;
    double compressionRatio;
    bool success;
};

class CompressionEngine : public QObject
{
    Q_OBJECT

public:
    explicit CompressionEngine(QObject *parent = nullptr);
    ~CompressionEngine();
    
    void compressFile(const QString &inputPath, const QString &outputPath, int level = 19);
    void decompressFile(const QString &inputPath, const QString &outputPath);
    bool isCompressed(const QString &filePath) const;
    CompressionStats getLastStats() const { return lastStats; }

signals:
    void compressionProgress(int percent);
    void compressionFinished(bool success);
    void compressionError(const QString &error);
    void compressionStats(const CompressionStats &stats);

private:
    bool compressData(const QByteArray &input, QByteArray &output, int level);
    bool decompressData(const QByteArray &input, QByteArray &output);
    
    ZSTD_CCtx *cctx;
    ZSTD_DCtx *dctx;
    CompressionStats lastStats;
};

#endif // COMPRESSIONENGINE_H
