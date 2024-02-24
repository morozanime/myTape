#ifndef IOTAPE_H
#define IOTAPE_H

#include <QThread>
#include "ASYNC_queue.h"
#include <qt_windows.h>
#include "tapecatalog.h"

//#define TAPE_EMULATION_FILE "$tmp$.tape"

class IOTape : public QThread
{
    Q_OBJECT

protected:
    void run(void) {
        while(true) {
            switch(state) {
            case TAPE_IDLE:
                if(_cmd_abort != cmd_abort) {
                    _cmd_abort = cmd_abort;
                    command.clear();
                    Flush();
                    break;
                }
                if(!command.isEmpty()) {
                    Command_t c = command.dequeue();
                    switch(c.cmd) {
                    case CMD_SEEK:
                        state = TAPE_SEEK;
                        emit progress(iTotal, nTotal, 0);
                        _io_seek_blocking(c.arg);
                        iTotal++;
                        emit progress(iTotal, nTotal, 100);
                        state = TAPE_IDLE;
                        break;
                    case CMD_READ:
                        emit progress(iTotal, nTotal, 0);
                        _total_bytes = c.arg;
                        _remaining_bytes = c.arg;
                        state = TAPE_READ;
                        break;
                    case CMD_WRITE:
                        emit progress(iTotal, nTotal, 0);
                        _total_bytes = c.arg;
                        _remaining_bytes = c.arg;
                        state = TAPE_WRITE;
                        break;
                    case CMD_SCAN:
                        emit progress(iTotal, nTotal, 0);
                        if(tapeCatalog != nullptr) {
                            delete tapeCatalog;
                        }
                        tapeCatalog = new TapeCatalog(mediaInfo.BlockSize);
                        state = TAPE_SCAN;
                        break;
                    case CMD_GETPOS:
                        GetPosition();
                        iTotal++;
                        emit change_pos();
                        emit progress(iTotal, nTotal, 100);
                        break;
                    }
                    break;
                }
                usleep(5000);
                break;
            case TAPE_READ:
                if(_remaining_bytes > 0) {
                    if(qReadBytes >= writeCacheSizeMax) {
                        continue;
                    }
                    Chunk_t c;
                    c.len = (uint32_t)(_remaining_bytes > max_chunk_len ? max_chunk_len : _remaining_bytes);
                    c.data = malloc(c.len);
                    if(c.data == nullptr) {
                        usleep(5000);
                        continue;
                    }
                    c.positionBytes = mediaPositionBytes;
                    if(_io_read_blocking(c.data, c.len)) {
                        free(c.data);
                        state = TAPE_ERROR;
                        emit error_signal(QString::number(mediaPositionBytes) + " Tape loading error");
                        break;
                    }
                    qRead.enqueue(c);
                    mediaPositionBytes += c.len;
                    emit change_pos();
                    _remaining_bytes -= c.len;
                    emit progress(iTotal, nTotal, (double)(_total_bytes - _remaining_bytes) / _total_bytes * 100);
                } else {
                    iTotal++;
                    emit progress(iTotal, nTotal, 100);
                    state = TAPE_IDLE;
                    break;
                }
                break;
            case TAPE_SCAN:
                if(_cmd_abort == cmd_abort) {
                    Chunk_t c;
                    c.len = max_chunk_len;
                    c.data = malloc(c.len);
                    if(c.data == nullptr) {
                        usleep(5000);
                        continue;
                    }
                    c.positionBytes = mediaPositionBytes;

                    if(_io_read_blocking(c.data, c.len)) {
                        free(c.data);
                        state = TAPE_ERROR;
                        emit error_signal(QString::number(mediaPositionBytes) + " Tape loading error");
                        break;
                    }
                    if(tapeCatalog->search(c.positionBytes, (const char*) c.data, c.len) >= 0) {
                        iTotal++;
                        emit progress(iTotal, nTotal, 100);
                        state = TAPE_IDLE;
                        emit catalog_readed(tapeCatalog);
                        mediaPositionBytes += c.len;
                    } else {
                        mediaPositionBytes += c.len;
                        emit progress(iTotal, nTotal, (double) (mediaPositionBytes * 100) / mediaInfo.Capacity.QuadPart);
                    }
                    emit change_pos();
                    free(c.data);
                } else {
                    delete tapeCatalog;
                    tapeCatalog = nullptr;
                    state = TAPE_IDLE;
                }
                break;
            case TAPE_WRITE:
                if(_remaining_bytes > 0) {
                    if(!qWrite.isEmpty()) {
                        Chunk_t c = qWrite.dequeue();
                        if(_cmd_abort == cmd_abort) {
                            if(_io_write_blocking(c.data, c.len)) {
                                state = TAPE_ERROR;
                                emit error_signal(QString::number(mediaPositionBytes / mediaInfo.BlockSize) + " Tape writing error");
                            } else {
                                mediaPositionBytes += c.len;
                                emit change_pos();
                                qWriteTotalBytesGet += c.len;
                                if(qWriteBytes < c.len) {
                                    qWriteBytes = 0;
                                } else {
                                    qWriteBytes -= c.len;
                                }
                                emit change_cache(qWriteBytes);
                                if(_remaining_bytes <= c.len) {
                                    _remaining_bytes = 0;
                                } else {
                                    _remaining_bytes -= c.len;
                                }
                                emit progress(iTotal, nTotal, (double)(_total_bytes - _remaining_bytes) / _total_bytes * 100, c.afp);
                            }
//                        emit log(0, QString("free %1").arg((uint64_t) c.data, 16, 16, QChar('0')));
                        }
                        free(c.data);
                    }
                } else {
                    iTotal++;
                    emit progress(iTotal, nTotal, 100);
                    state = TAPE_IDLE;
                }
                break;
            case TAPE_ERROR:
                cmd_abort++;
                break;
            default:
                if(_cmd_abort != cmd_abort) {
                    _cmd_abort = cmd_abort;
                    command.clear();
                    break;
                }
                break;
            }
        }
    }

public:
    IOTape() {
        memset(&mediaInfo, 0, sizeof(mediaInfo));
        memset(&driveInfo, 0, sizeof(driveInfo));
        mediaPositionBytes = 0;
        nullTape = false;
    }

    ~IOTape() {
        if(hTape != INVALID_HANDLE_VALUE) {
            CloseHandle(hTape);
            hTape = INVALID_HANDLE_VALUE;
        }
        _flush(&qRead);
        _flush(&qWrite);
        if(tapeCatalog != nullptr) {
            delete tapeCatalog;
        }
    }

    int Open(const char * device, int buffLen = 0);
    void Close(void);

    typedef enum {
        CMD_SEEK, CMD_READ, CMD_WRITE, CMD_SCAN, CMD_GETPOS,
    } E_Command;

    typedef enum {
        TAPE_IDLE, TAPE_READ, TAPE_WRITE, TAPE_SEEK, TAPE_SCAN, TAPE_ERROR
    } E_State;

    E_State GetState(void) {
        return state;
    }

    void Abort(void) {
        cmd_abort++;
    }

    void Flush(void) {
        if(state == TAPE_IDLE) {
            _flush(&qRead);
            _flush(&qWrite);
            nTotal = 0;
            iTotal = 0;
            qWriteTotalBytesGet = 0;
            qWriteTotalBytesPut = 0;
            qReadBytes = 0;
            qWriteBytes = 0;
            emit change_cache(0);
        }
    }

    void Command(E_Command cmd, uint64_t arg = 0) {
        nTotal++;
        Command_t c;
        c.cmd = cmd;
        c.arg = arg;
        command.enqueue(c);
    }

    int Write(void * data, uint32_t len, QString afp) {
        uint32_t writeCacheSize = qWriteTotalBytesPut - qWriteTotalBytesGet;
        if((uint64_t) writeCacheSize + len > (uint64_t) writeCacheSizeMax) {
            return 1;
        }
        Chunk_t c;
        c.data = malloc(len);
//        emit log(0, QString("malloc %1").arg((uint64_t) c.data, 16, 16, QChar('0')));
        if(c.data == nullptr) {
            return -1;
        }
        c.len = len;
        memcpy(c.data, data, c.len);
        c.afp = afp;
        qWrite.enqueue(c);
        qWriteBytes += c.len;
        qWriteTotalBytesPut += len;
        emit change_cache(qWriteBytes);
        return 0;
    }

    int Read(void * data, uint32_t * len, uint64_t * positionBytes) {
        if(qRead.isEmpty())
            return -1;
        Chunk_t c = qRead.first();
        if(len != nullptr)
            *len = c.len;
        if(positionBytes != nullptr)
            *positionBytes = c.positionBytes;
        if(data != nullptr) {
            memcpy(data, c.data, c.len);
            free(c.data);
            qRead.dequeue();
            if(qReadBytes > c.len) {
                qReadBytes -= c.len;
            } else {
                qReadBytes = 0;
            }
            emit change_cache(qReadBytes);
        }
        return 0;
    }

    bool isOpened(void) {
        if(nullTape) {
            return true;
        } else {
            return hTape != INVALID_HANDLE_VALUE;
        }
    }

    uint64_t RoundUp(uint64_t size) {
        return (uint64_t)(size + mediaInfo.BlockSize - 1) & ~(uint64_t)(mediaInfo.BlockSize - 1);
    }

    uint32_t max_chunk_len = 4 * 1024 * 1024;
    uint32_t writeCacheSizeMax = 256 * 1024 * 1024;

    TAPE_GET_MEDIA_PARAMETERS mediaInfo;
    TAPE_GET_DRIVE_PARAMETERS driveInfo;
    uint64_t mediaPositionBytes = 0;
    TapeCatalog * tapeCatalog = nullptr;

    uint64_t GetPosition(void) {
#ifdef  TAPE_EMULATION_FILE
#else   /*TAPE_EMULATION_FILE*/
        if(!nullTape) {
            DWORD dwPartition = 0;
            DWORD dwOffsetLow = 0;
            DWORD dwOffsetHigh = 0;
            GetTapePosition(hTape, TAPE_ABSOLUTE_POSITION, &dwPartition, &dwOffsetLow, &dwOffsetHigh);

            mediaPositionBytes = (((uint64_t) dwOffsetHigh << 32) | dwOffsetLow) * mediaInfo.BlockSize;
        }
#endif  /*TAPE_EMULATION_FILE*/
        return mediaPositionBytes;
    }

signals:
    void progress(int iTotal, int nTotal, double percent, QString str = "");
    void catalog_readed(TapeCatalog * catalog);
    void change_pos(void);
    void error_signal(QString message);
    void change_cache(uint64_t size);
    void log(int level, QString message);

private:
    HANDLE hTape = INVALID_HANDLE_VALUE;
    bool nullTape = false;

    typedef struct {
        void * data;
        uint32_t len;
        uint64_t positionBytes;
        QString afp;
    } Chunk_t;

    QAsyncQueue<Chunk_t> qRead;
    uint64_t qReadBytes = 0;

    QAsyncQueue<Chunk_t> qWrite;
    uint64_t qWriteBytes = 0;
    uint32_t qWriteTotalBytesPut = 0;
    uint32_t qWriteTotalBytesGet = 0;

    E_State state = TAPE_IDLE;
    int cmd_abort = 0;
    int _cmd_abort = 0;

    void _flush(QAsyncQueue<Chunk_t> * q) {
        while(!q->isEmpty()) {
            Chunk_t c = q->dequeue();
            if(c.data != nullptr) {
                free(c.data);
            }
        }
    }

    typedef struct {
        E_Command cmd;
        uint64_t arg;
    } Command_t;

    QAsyncQueue<Command_t> command;

    int _io_read_blocking(void * dest, uint32_t len);
    int _io_write_blocking(void * src, uint32_t len);
    int _io_seek_blocking(uint64_t pos);

    uint64_t _total_bytes = 0;
    uint64_t _remaining_bytes = 0;
    int iTotal = 0;
    int nTotal = 0;

};

#endif // IOTAPE_H
