#include "compressionengine.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <cstring>

const int CHUNK_SIZE = 1024 * 1024; // 1MB chunks
const char MAGIC_NUMBER[] = "ZSTD";

CompressionEngine::CompressionEngine(QObject *parent)
    : QObject(parent)
{
    cctx = ZSTD_createCCtx();
    dctx = ZSTD_createDCtx();
    
    if (!cctx || !dctx) {
        emit compressionError("Failed to initialize compression context");
    }
}

CompressionEngine::~CompressionEngine()
{
    if (cctx) ZSTD_freeCCtx(cctx);
    if (dctx) ZSTD_freeDCtx(dctx);
}

void CompressionEngine::compressFile(const QString &inputPath, const QString &outputPath, int level)
{
    QFile inputFile(inputPath);
    QFile outputFile(outputPath);
    
    if (!inputFile.open(QIODevice::ReadOnly)) {
        emit compressionError("Cannot open input file: " + inputPath);
        return;
    }
    
    if (!outputFile.open(QIODevice::WriteOnly)) {
        emit compressionError("Cannot open output file: " + outputPath);
        inputFile.close();
        return;
    }
    
    // Write magic number
    outputFile.write(MAGIC_NUMBER, 4);
    
    qint64 originalSize = inputFile.size();
    qint64 totalRead = 0;
    qint64 totalWritten = 4; // Magic number size
    
    try {
        while (!inputFile.atEnd()) {
            QByteArray chunk = inputFile.read(CHUNK_SIZE);
            if (chunk.isEmpty()) break;
            
            QByteArray compressedChunk;
            if (!compressData(chunk, compressedChunk, level)) {
                throw std::runtime_error("Compression failed");
            }
            
            // Write chunk size (4 bytes) + compressed chunk
            quint32 chunkSize = compressedChunk.size();
            outputFile.write((char*)&chunkSize, 4);
            outputFile.write(compressedChunk);
            
            totalRead += chunk.size();
            totalWritten += 4 + compressedChunk.size();
            
            // Emit progress
            int progress = (totalRead * 100) / originalSize;
            emit compressionProgress(progress);
        }
        
        inputFile.close();
        outputFile.close();
        
        // Prepare stats
        lastStats.filePath = inputPath;
        lastStats.originalSize = originalSize;
        lastStats.compressedSize = totalWritten;
        lastStats.compressionRatio = (double)totalWritten / originalSize;
        lastStats.success = true;
        
        emit compressionStats(lastStats);
        emit compressionProgress(100);
        emit compressionFinished(true);
        
    } catch (const std::exception &e) {
        inputFile.close();
        outputFile.close();
        outputFile.remove();
        
        lastStats.success = false;
        emit compressionError("Compression error: " + QString::fromStdString(e.what()));
        emit compressionFinished(false);
    }
}

void CompressionEngine::decompressFile(const QString &inputPath, const QString &outputPath)
{
    QFile inputFile(inputPath);
    QFile outputFile(outputPath);
    
    if (!inputFile.open(QIODevice::ReadOnly)) {
        emit compressionError("Cannot open input file: " + inputPath);
        return;
    }
    
    if (!outputFile.open(QIODevice::WriteOnly)) {
        emit compressionError("Cannot open output file: " + outputPath);
        inputFile.close();
        return;
    }
    
    try {
        // Check magic number
        char magicBuffer[4];
        if (inputFile.read(magicBuffer, 4) != 4 || std::memcmp(magicBuffer, MAGIC_NUMBER, 4) != 0) {
            throw std::runtime_error("Invalid compression format");
        }
        
        qint64 inputSize = inputFile.size();
        qint64 totalRead = 4;
        
        while (totalRead < inputSize) {
            // Read chunk size
            char sizeBuffer[4];
            if (inputFile.read(sizeBuffer, 4) != 4) break;
            
            quint32 chunkSize;
            std::memcpy(&chunkSize, sizeBuffer, 4);
            totalRead += 4;
            
            if (chunkSize > 10 * CHUNK_SIZE) { // Safety check
                throw std::runtime_error("Invalid chunk size");
            }
            
            // Read compressed chunk
            QByteArray compressedChunk = inputFile.read(chunkSize);
            if (compressedChunk.size() != chunkSize) break;
            
            totalRead += chunkSize;
            
            // Decompress
            QByteArray decompressedChunk;
            if (!decompressData(compressedChunk, decompressedChunk)) {
                throw std::runtime_error("Decompression failed");
            }
            
            outputFile.write(decompressedChunk);
            
            // Emit progress
            int progress = (totalRead * 100) / inputSize;
            emit compressionProgress(progress);
        }
        
        inputFile.close();
        outputFile.close();
        
        emit compressionProgress(100);
        emit compressionFinished(true);
        
    } catch (const std::exception &e) {
        inputFile.close();
        outputFile.close();
        outputFile.remove();
        
        emit compressionError("Decompression error: " + QString::fromStdString(e.what()));
        emit compressionFinished(false);
    }
}

bool CompressionEngine::isCompressed(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    char buffer[4];
    bool isCompressed = (file.read(buffer, 4) == 4) && (std::memcmp(buffer, MAGIC_NUMBER, 4) == 0);
    file.close();
    
    return isCompressed;
}

bool CompressionEngine::compressData(const QByteArray &input, QByteArray &output, int level)
{
    if (input.isEmpty()) {
        return false;
    }
    
    size_t maxSize = ZSTD_compressBound(input.size());
    output.resize(maxSize);
    
    size_t compressedSize = ZSTD_compressCCtx(
        cctx,
        output.data(),
        output.size(),
        input.data(),
        input.size(),
        level
    );
    
    if (ZSTD_isError(compressedSize)) {
        return false;
    }
    
    output.resize(compressedSize);
    return true;
}

bool CompressionEngine::decompressData(const QByteArray &input, QByteArray &output)
{
    if (input.isEmpty()) {
        return false;
    }
    
    // Get decompressed size
    unsigned long long decompressedSize = ZSTD_getFrameContentSize(input.data(), input.size());
    
    if (ZSTD_isError(decompressedSize)) {
        return false;
    }
    
    if (decompressedSize > 100 * CHUNK_SIZE) { // Safety check - max 100MB per chunk
        return false;
    }
    
    output.resize(decompressedSize);
    
    size_t result = ZSTD_decompressDCtx(
        dctx,
        output.data(),
        output.size(),
        input.data(),
        input.size()
    );
    
    if (ZSTD_isError(result)) {
        return false;
    }
    
    return true;
}
