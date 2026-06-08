#include "diskanalyzer.h"
#include <QDir>
#include <QFileInfo>
#include <QStorageInfo>
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>

DiskAnalyzer::DiskAnalyzer(QObject *parent)
    : QObject(parent)
{
}

void DiskAnalyzer::analyzeDisk(const QString &drivePath)
{
    analysisResult.files.clear();
    fileHashes.clear();
    
    // Get disk info
    QStorageInfo storage(drivePath);
    analysisResult.driveLetter = drivePath;
    analysisResult.totalSize = storage.bytesTotal();
    analysisResult.usedSize = storage.bytesTotal() - storage.bytesFree();
    analysisResult.freeSize = storage.bytesFree();
    analysisResult.usagePercent = (double)analysisResult.usedSize / analysisResult.totalSize * 100;
    
    // Analyze directory
    emit analysisProgress(10);
    analyzeDirectory(drivePath, analysisResult.files);
    
    emit analysisProgress(50);
    
    // Find duplicates
    findDuplicates(analysisResult.files);
    
    emit analysisProgress(80);
    
    // Calculate recoverable space
    analysisResult.recoverableSpace = calculateRecoverableSpace();
    
    emit analysisProgress(100);
    emit analysisFinished();
}

void DiskAnalyzer::analyzeDirectory(const QString &path, QVector<FileInfo> &files)
{
    QDir dir(path);
    
    // Skip system directories
    QStringList skipDirs = {"System32", "Program Files", "Windows", "ProgramData", ".git", "node_modules"};
    
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::NoSort);
    
    for (const QFileInfo &entry : entries) {
        // Skip system directories
        if (skipDirs.contains(entry.fileName())) {
            continue;
        }
        
        if (entry.isDir()) {
            // Recursively analyze subdirectory
            analyzeDirectory(entry.absoluteFilePath(), files);
        } else if (entry.isFile()) {
            FileInfo fileInfo;
            fileInfo.path = entry.absoluteFilePath();
            fileInfo.name = entry.fileName();
            fileInfo.size = entry.size();
            fileInfo.type = getFileType(fileInfo.path);
            fileInfo.isDuplicate = false;
            fileInfo.lastAccessed = entry.lastRead();
            
            // Only include files larger than 1MB
            if (fileInfo.size > 1024 * 1024) {
                files.append(fileInfo);
            }
        }
    }
}

QString DiskAnalyzer::getFileType(const QString &filePath)
{
    QString suffix = QFileInfo(filePath).suffix().toLower();
    
    // Video files
    if (suffix == "mp4" || suffix == "avi" || suffix == "mkv" || 
        suffix == "mov" || suffix == "flv" || suffix == "wmv" ||
        suffix == "webm" || suffix == "m4v") {
        return "video";
    }
    
    // Photo files
    if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || 
        suffix == "gif" || suffix == "bmp" || suffix == "tiff" ||
        suffix == "raw" || suffix == "svg" || suffix == "webp") {
        return "photo";
    }
    
    // Document files
    if (suffix == "pdf" || suffix == "docx" || suffix == "doc" || 
        suffix == "xlsx" || suffix == "pptx" || suffix == "txt" ||
        suffix == "rtf" || suffix == "odt") {
        return "document";
    }
    
    // Archive files
    if (suffix == "zip" || suffix == "rar" || suffix == "7z" || 
        suffix == "tar" || suffix == "gz" || suffix == "bz2") {
        return "archive";
    }
    
    return "other";
}

qint64 DiskAnalyzer::getDirectorySize(const QString &path)
{
    qint64 size = 0;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            size += getDirectorySize(entry.absoluteFilePath());
        } else {
            size += entry.size();
        }
    }
    
    return size;
}

void DiskAnalyzer::findDuplicates(QVector<FileInfo> &files)
{
    // Simple duplicate detection based on file size and name
    // In production, use MD5/SHA256 hashing for accurate detection
    
    QMap<QString, QVector<int>> sizeMap;
    
    for (int i = 0; i < files.count(); i++) {
        QString sizeKey = QString::number(files[i].size);
        sizeMap[sizeKey].append(i);
    }
    
    for (auto it = sizeMap.begin(); it != sizeMap.end(); ++it) {
        if (it.value().count() > 1) {
            // Potential duplicates found
            for (int i = 1; i < it.value().count(); i++) {
                files[it.value()[i]].isDuplicate = true;
                files[it.value()[i]].duplicateOf = files[it.value()[0]].path;
            }
        }
    }
}

qint64 DiskAnalyzer::calculateRecoverableSpace()
{
    qint64 recoverable = 0;
    
    for (const FileInfo &file : analysisResult.files) {
        // Assume video/photo compression ratio
        if (file.type == "video") {
            recoverable += file.size * 0.3; // 30% recovery from video compression
        } else if (file.type == "photo") {
            recoverable += file.size * 0.2; // 20% recovery from photo compression
        } else if (file.type == "document") {
            recoverable += file.size * 0.5; // 50% recovery from document compression
        } else if (file.isDuplicate) {
            recoverable += file.size; // Full recovery from duplicate removal
        }
    }
    
    return recoverable;
}
