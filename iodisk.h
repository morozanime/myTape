#ifndef IODISK_H
#define IODISK_H

#include <QQueue>
#include <QFileInfo>
#include <QThread>
#include <QDir>
#include <QElapsedTimer>
#include "iotape.h"

class IODisk : public QThread
{
    Q_OBJECT

protected:
    inline void process_restore()
    {
        if(restoreFileIndex >= restoreCatalog->filesOnTape.length()) {
            if(restoreFileCurrent != nullptr) {
                emit log(1, "close last");
                restoreFileCurrent->close();
                restoreFileCurrent = nullptr;
            }
            restoreCatalog = nullptr;
            return;
        }
        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
            if(buff == nullptr) {
                usleep(5000);
                return;
            }
        }
        uint32_t n = 0;
        uint64_t buffStart = 0;
        if(ioTape->Read(buff, &n, &buffStart) != 0 || n == 0) {
            return;
        }
        while(restoreFileIndex < restoreCatalog->filesOnTape.length()) {
            TapeCatalog::fileOnTape_t f = restoreCatalog->filesOnTape.value(restoreFileIndex);
            emit log(1, "pos:" + QString::number(buffStart) + " offset:" + QString::number(f.offset) + " fileSize:" + QString::number(f.fileSize) + " n:" + QString::number(n) + " restoreFileIndex:" + QString::number(restoreFileIndex));
            if(f.fileAttr & 1) {
                restoreFileIndex++;
                continue;
            }
            if(restoreFileCurrent != nullptr) {
                if(f.offset + f.fileSize > buffStart + n) {
                    emit log(1, "write full chunk ");
                    restoreFileCurrent->write((const char *) buff, n);
                    break;
                } else {
                    emit log(1, "write end " + QString::number((f.offset + f.fileSize) - buffStart) + " bytes");
                    restoreFileCurrent->write((const char *) buff, (f.offset + f.fileSize) - buffStart);
                    restoreFileCurrent->close();
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                }
            } else if(f.skip) {
                restoreFileIndex++;
                continue;
            } else if(f.offset >= buffStart && f.offset < buffStart + n) {
                emit log(1, "create");
                //create dirs
                QString a0 = f.fileNamePath;
                QDir root = QDir(restoreRootDir);
                if(a0.contains(":/"))
                    a0 = f.fileNamePath.split(":/", Qt::SkipEmptyParts).last();
                else if(a0.contains("//"))
                    a0 = f.fileNamePath.split("//", Qt::SkipEmptyParts).last();
                QStringList a1 = a0.split("/");
                QString name = a1.last();
                a1.removeLast();
                QString subPath = a1.join("/");
                QString fullPathName = restoreRootDir + "/" + subPath + "/" + name;
                if(!root.mkpath(subPath)) {
                    restoreFileIndex++;
                    continue;
                }
                //open file
                emit log(1, fullPathName);
                restoreFileCurrent = new QFile(fullPathName);
                if(!restoreFileCurrent->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate/* | QFile::OpenModeFlag::Unbuffered*/)) {
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                    continue;
                }
                uint64_t l = f.fileSize;
                uint64_t buffLeft = f.offset - buffStart;
                uint64_t buffRight = (buffStart + n) - f.offset;
                if(l > buffRight) {
                    l = buffRight;
                }
                restoreFileCurrent->write((const char *) buff + buffLeft, l);
                if(f.fileSize <= l) {
                    emit log(1, "close");
                    restoreFileCurrent->close();
                    restoreFileCurrent = nullptr;
                    restoreFileIndex++;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
}

    inline void process_backup()
    {
        QElapsedTimer timer;

        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
            if(buff == nullptr) {
                usleep(5000);
                return;
            }
        }

        timer.start();

        QFileInfo info = filesToWrite.dequeue();
        if(info.isDir())
            return;
        QString afp = info.absoluteFilePath();
        QFile f(afp);
        if(info.size() == 0)
            return;

        timers_ms[0] += timer.nsecsElapsed();
        timer.start();

        if(f.open(QFile::OpenModeFlag::ReadOnly/* | QFile::OpenModeFlag::Unbuffered*/)) {
            timers_ms[5]++;

            timers_ms[1] += timer.nsecsElapsed();
            timer.start();

            quint64 fileSizeCounter = 0;
            do {
                quint32 n0 = f.read((char*)buff, ioTape->max_chunk_len);
                timers_ms[6] += n0;

                timers_ms[2] += timer.nsecsElapsed();
                timer.start();

                if(n0 == 0)
                    break;
                quint32 n = n0 + ioTape->mediaInfo.BlockSize - 1;
                n &= ~(ioTape->mediaInfo.BlockSize - 1);
                if(n > n0)
                    memset((uint8_t*)buff + n0, 0, n - n0);
                int res = 0;
                while(!cmd_flush) {
                    timers_ms[3] += timer.nsecsElapsed();
                    timer.start();

                    res = ioTape->Write(buff, n, fileSizeCounter, afp);
                    if(filesToWrite.isEmpty()) {
                        if(ioTape->paused) {
                            emit log(0, "Last file readed. Start write.");
                        }
                        ioTape->paused = false;
                    }
                    timers_ms[4] += timer.nsecsElapsed();
                    timer.start();

                    if(res == 0) {
                        break;
                    }
                    usleep(5000);
                }
                fileSizeCounter += n0;
            } while(!cmd_flush);
            f.close();
            if(!cmd_flush && (qint64) fileSizeCounter != info.size()) {
                cmd_flush = true;
                emit error_signal(afp + " reading error at " + QString::number(fileSizeCounter) + "/" + QString::number(info.size()));
                return;
            }
        } else {
            cmd_flush = true;
            emit error_signal(afp + " open error");
            return;
        }
}

    inline void process_rawread()
    {
        if(buff == nullptr) {
            buff = malloc(ioTape->max_chunk_len);
            if(buff == nullptr) {
                usleep(5000);
                return;
            }
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
                emit log(0, QString("-------------------"));
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
        if(restoreFileCurrent != nullptr) {
            emit log(1, "close last (destructor)");
            restoreFileCurrent->close();
            restoreFileCurrent = nullptr;
        }
        if(rawFile != nullptr) {
            rawFile->close();
            rawFile = nullptr;
        }
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
            if(!rawFile->open(QFile::OpenModeFlag::WriteOnly/* | QFile::OpenModeFlag::Unbuffered*/))
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
        for(auto i = 0; i < catalog->filesOnTape.count(); i++) {
            catalog->filesOnTape[i].skip = false;
        }
        ioTape->Command(IOTape::CMD_SEEK, catalog->offsetOnTape);
        ioTape->Command(IOTape::CMD_READ, catalog->totalSize);
    }

    void Restore(QString root, TapeCatalog * catalog, QList<int> selected) {
        restoreFileIndex = 0;
        restoreCatalog = catalog;
        restoreRootDir = root;
        for(auto i = 0; i < catalog->filesOnTape.count(); i++) {
            catalog->filesOnTape[i].skip = true;
        }
        for(auto i = 0; i < selected.count(); i++) {
            if(selected.value(i) >= catalog->filesOnTape.count())
                continue;
            TapeCatalog::fileOnTape_t f = catalog->filesOnTape.value(selected.value(i));
            catalog->filesOnTape[selected[i]].skip = false;
            if(f.fileSize > 0) {
                ioTape->Command(IOTape::CMD_SEEK, f.offset);
                ioTape->Command(IOTape::CMD_READ, ioTape->RoundUp(f.fileSize));
            }
        }
    }

    qint64 timers_ms[10] = { 0 };

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
