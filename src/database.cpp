#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Database::Database()
{
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::initialize()
{
    // Get application data directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    
    // Create database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataPath + "/compression.db");
    
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    
    // Create table if not exists
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS compression_history ("
               "id INTEGER PRIMARY KEY,"
               "file_path TEXT NOT NULL,"
               "original_size INTEGER NOT NULL,"
               "compressed_size INTEGER NOT NULL,"
               "compression_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
               ");");
    
    return true;
}

void Database::saveCompressionRecord(const CompressionRecord &record)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return;
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO compression_history (file_path, original_size, compressed_size) "
                  "VALUES (:file_path, :original_size, :compressed_size)");
    query.addBindValue(record.filePath);
    query.addBindValue(record.originalSize);
    query.addBindValue(record.compressedSize);
    
    if (!query.exec()) {
        qDebug() << "Failed to insert record:" << query.lastError().text();
    }
}

QVector<CompressionRecord> Database::getCompressionHistory()
{
    QVector<CompressionRecord> records;
    
    if (!db.isOpen()) {
        return records;
    }
    
    QSqlQuery query("SELECT file_path, original_size, compressed_size, compression_date "
                    "FROM compression_history ORDER BY compression_date DESC LIMIT 100");
    
    while (query.next()) {
        CompressionRecord record;
        record.filePath = query.value(0).toString();
        record.originalSize = query.value(1).toLongLong();
        record.compressedSize = query.value(2).toLongLong();
        record.compressionDate = query.value(3).toString();
        records.append(record);
    }
    
    return records;
}

void Database::clearHistory()
{
    if (!db.isOpen()) {
        return;
    }
    
    QSqlQuery query;
    query.exec("DELETE FROM compression_history");
}
