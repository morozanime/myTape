#ifndef IODISK_H
#define IODISK_H

#include <QQueue>
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include "iotape.h"

class IODisk : public QThread
{
    Q_OBJECT

protected:
    inline void process_restore()
    {
        if(restoreFileIndex >= restoreCatalog->filesOnTape.length()) {
            restoreCatalog = nullptr;
            return;
        }
        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
        }
        uint32_t n = 0;
        uint64_t restorePosBytes = 0;
        if(ioTape->Read(buff, &n, &restorePosBytes) != 0 || n == 0) {
            return;
        }
        while(restoreFileIndex < restoreCatalog->filesOnTape.length()) {
            TapeCatalog::fileOnTape_t f = restoreCatalog->filesOnTape.value(restoreFileIndex);
//                        emit log(1, "pos:" + QString::number(restorePosBytes) + " offset:" + QString::number(f.offset) + " fileSize:" + QString::number(f.fileSize) + " n:" + QString::number(n) + " restoreFileIndex:" + QString::number(restoreFileIndex));
            if(restoreFileCurrent != nullptr) {
                if(f.offset + f.fileSize > restorePosBytes + n) {
                    restoreFileCurrent->write((const char *) buff, n);
                    break;
                } else {
                    restoreFileCurrent->write((const char *) buff, (f.offset + f.fileSize) - restorePosBytes);
                    restoreFileCurrent->close();
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                }
            } else if(f.offset >= restorePosBytes && f.offset < restorePosBytes + n) {
//                            emit log(1, "create");
                //create dirs
                QString a0 = f.fileNamePath;
                if(a0.contains(":/"))
                    a0 = f.fileNamePath.split(":/", Qt::SkipEmptyParts).last();
                QStringList a1 = a0.split("/");
                QString name = a1.last();
                a1.removeLast();
                QString subPath = a1.join("/");
                QString fullPathName = restoreRootDir + "/" + subPath + "/" + name;
                QDir root = QDir(restoreRootDir);
                if(!root.mkpath(subPath)) {
                    restoreFileIndex++;
                    continue;
                }
                //open file
//                            emit log(1, fullPathName);
                restoreFileCurrent = new QFile(fullPathName);
                if(!restoreFileCurrent->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate | QFile::OpenModeFlag::Unbuffered)) {
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                    continue;
                }
                uint64_t l = f.fileSize;
                if(l > n - (f.offset - restorePosBytes)) {
                    l = n - (f.offset - restorePosBytes);
                }
                restoreFileCurrent->write((const char *) buff + f.offset - restorePosBytes, l);
                if(f.fileSize <= l) {
                    restoreFileCurrent->close();
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                }
            } else {
                break;
            }
        }
}

    inline void process_backup()
    {
        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
        }
        QFileInfo info = filesToWrite.dequeue();
        QFile f(info.absoluteFilePath());
        if(info.size() == 0)
            return;
        if(f.open(QFile::OpenModeFlag::ReadOnly | QFile::OpenModeFlag::Unbuffered)) {
            qint64 fileSizeCounter = 0;
            do {
                uint64_t n0 = f.read((char*)buff, ioTape->max_chunk_len);
                if(n0 == 0)
                    break;
                fileSizeCounter += n0;
                uint64_t n = n0 + ioTape->mediaInfo.BlockSize - 1;
                n &= ~(ioTape->mediaInfo.BlockSize - 1);
                if(n > n0)
                    memset((uint8_t*)buff + n0, 0, n - n0);
                int res = 0;
                while(!cmd_flush) {
                    res = ioTape->Write(buff, n);
                    if(res == 0) {
                        break;
                    }
                    usleep(5000);
                }
            } while(!cmd_flush);
            if(!cmd_flush && fileSizeCounter != info.size()) {
                cmd_flush = 1;
                emit error_signal(info.absoluteFilePath() + " reading error at " + QString::number(fileSizeCounter) + "/" + QString::number(info.size()));
                return;
            }
        } else {
            cmd_flush = 1;
            emit error_signal(info.absoluteFilePath() + " open error");
            return;
        }
}

    inline void process_rawread()
    {
        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
        }
        uint32_t n = 0;
        if(ioTape->Read(buff, &n, nullptr) == 0 && n > 0) {
            uint64_t n1 = rawFile->write((const char*) buff, n);
            if(n1 == 0) {
                cmd_flush = 1;
                emit error_signal("Write file error");
            }
            rawFileWritten += n1;
        }
        if(rawFileWritten >= rawFileSize || cmd_flush) {
            rawFile->close();
            delete rawFile;
            rawFile = nullptr;
        }
}

    void run(void) {
        while(true) {
            if(cmd_flush) {
                filesToWrite.clear();
                cmd_flush = false;
            }
            if(!filesToWrite.isEmpty()) {
                process_backup();
            } else if(rawFile != nullptr && rawFile->isOpen()) {
                process_rawread();
            } else if(restoreCatalog != nullptr && !restoreRootDir.isEmpty()) {
                process_restore();
            } else {
                if(buff != nullptr) {
                    free(buff);
                    buff = nullptr;
                }
                usleep(5000);
            }
        }
    }

public:
    IODisk(IOTape * ioTape){
        this->ioTape = ioTape;
    }

    ~IODisk(){

    }

    void Write(QFileInfo f) {
        filesToWrite.enqueue(f);
    }

    void Flush(void) {
        cmd_flush = true;
    }

    int Read(QString fn, uint64_t len) {
        do {
            rawFile = new QFile(fn);
            if(rawFile == nullptr)
                break;
            if(!rawFile->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Unbuffered))
                break;
            rawFileSize = len;
            rawFileWritten = 0;
            ioTape->Command(IOTape::CMD_READ, len);
            return 0;
        } while(0);
        if(rawFile != nullptr){
            delete rawFile;
            rawFile = nullptr;
        }
        return -1;
    }

    void Restore(QString root, TapeCatalog * catalog) {
        restoreFileIndex = 0;
        restoreCatalog = catalog;
        restoreRootDir = root;
        ioTape->Command(IOTape::CMD_SEEK, catalog->offsetOnTape);
        ioTape->Command(IOTape::CMD_READ, catalog->totalSize);
    }

signals:
    void error_signal(QString message);
    void log(int level, QString message);

private:
    IOTape * ioTape;
    QQueue<QFileInfo> filesToWrite;
    void * buff = nullptr;
    bool cmd_flush = false;

    QFile * rawFile = nullptr;
    uint64_t rawFileSize = 0;
    uint64_t rawFileWritten = 0;

    TapeCatalog * restoreCatalog = nullptr;
    QString restoreRootDir;
    int restoreFileIndex = 0;
    QFile * restoreFileCurrent = nullptr;
};

#endif // IODISK_H
