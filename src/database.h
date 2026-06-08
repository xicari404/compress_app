#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QVector>
#include <QSqlDatabase>

struct CompressionRecord {
    QString filePath;
    qint64 originalSize;
    qint64 compressedSize;
    QString compressionDate;
};

class Database
{
public:
    Database();
    ~Database();
    
    bool initialize();
    void saveCompressionRecord(const CompressionRecord &record);
    QVector<CompressionRecord> getCompressionHistory();
    void clearHistory();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
