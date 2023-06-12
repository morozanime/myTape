#ifndef WORKER_WRITEFILELIST_H
#define WORKER_WRITEFILELIST_H

#include <QFileInfo>
#include <QList>
#include <QThread>
#include <QDir>
#include <QDirIterator>
#include "ASYNC_queue.h"
#include "tapecatalog.h"
#include <qt_windows.h>

class Worker_WriteFileList : public QThread
{

    Q_OBJECT

public:
    typedef enum {
        CMD_ADD_FILES, CMD_ADD_DIR, CMD_CLEAR,
    } E_Command;

    typedef enum {
        STATE_IDLE, STATE_BUSY,
    } E_State;

    Worker_WriteFileList() {
    }

    ~Worker_WriteFileList() {
        if(catalog != nullptr) {
            delete catalog;
        }
    }

    uint32_t blockSize = 0;
    QByteArray catalog_serialized;
    QList<QFileInfo> filesToWrite;
    TapeCatalog * catalog = nullptr;

    void Cmd_AddFile(QStringList fileNames) {
        Command_t cmd;
        cmd.cmd = CMD_ADD_FILES;
        cmd.fileNames = fileNames;
        command.enqueue(cmd);
    }

    void Cmd_AddDir(QString dir) {
        Command_t cmd;
        cmd.cmd = CMD_ADD_DIR;
        cmd.dir = dir;
        command.enqueue(cmd);
    }

    void Cmd_Clear(void) {
        Command_t cmd;
        cmd.cmd = CMD_CLEAR;
        command.enqueue(cmd);
    }

signals:
    void AddFile(QString path, qint64 size);
    void CatalogReady(qint64 total_size);
    void Clear(void);
    void progress(int iTotal, int nTotal, double percent);

protected:
    void run(void) {
        while(true) {
            switch(state) {
            case STATE_IDLE:
                if(!command.isEmpty()) {
                    Command_t cmd = command.dequeue();
                    if(cmd.cmd == CMD_ADD_FILES) {
                        for(int i = 0; i < cmd.fileNames.length(); i++) {
                            QFileInfo f(cmd.fileNames.value(i));
                            filesToWrite.append(f);
                            emit AddFile(f.absoluteFilePath(), f.size());
                        }
                    } else if(cmd.cmd == CMD_ADD_DIR) {
                        _add0(cmd.dir);
                    } else if(cmd.cmd == CMD_CLEAR) {
                        filesToWrite.clear();
                        catalog_serialized.clear();
                        emit Clear();
                    }

                    if(command.isEmpty()) {
                        if(catalog != nullptr) {
                            delete catalog;
                        }
                        catalog = new TapeCatalog(blockSize, filesToWrite);
                        catalog_serialized = catalog->serialize();
                        emit CatalogReady(catalog->totalSize);
                    }
                }
                usleep(5000);
                break;
            default:
                break;
            }
        }
    }

private:
    void _add0(QString dir) {
        QDirIterator * itFiles = new QDirIterator(dir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Modified  | QDir::Hidden | QDir::System );
        while (itFiles->hasNext()) {
            itFiles->next();
            QFileInfo f = itFiles->fileInfo();
            filesToWrite.append(f);
            if(f.isDir()) {
                _add0(f.absoluteFilePath());
            } else {
                emit AddFile(f.absoluteFilePath(), f.size());
            }
        }
        delete itFiles;
    }

    typedef struct {
        E_Command cmd;
        QString dir;
        QStringList fileNames;
    } Command_t;

    QAsyncQueue<Command_t> command;

    E_State state = STATE_IDLE;

};

#endif // WORKER_WRITEFILELIST_H
