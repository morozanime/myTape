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
//            if(hTape == INVALID_HANDLE_VALUE) {
//                usleep(50000UL);
//                tapeStatus = 8888;
//                emit status(tapeStatus);
//                continue;
//            }

//            emit progress1(100, clock()/*, "hello!"*/);

            if(!nullTape && hTape != INVALID_HANDLE_VALUE) {
                quint32 st = GetTapeStatus(hTape);
                if(st != tapeStatus) {
                    emit status(st);
                    tapeStatus = st;
                }
                if(tapeStatus != 0) {
                    usleep(50000UL);
                    continue;
                }
            }
            if(_cmd_abort != cmd_abort) {
                _cmd_abort = cmd_abort;
                command.clear();
                Flush();
                emit progress(0, "ABORT", 0, true);
                emit change_pos(true);
                state = TAPE_IDLE;
                usleep(50000UL);
                continue;
            }
            if(state == TAPE_IDLE) {
                if(!command.isEmpty()) {
                    Command_t c = command.dequeue();
                    switch(c.cmd) {
                    case CMD_SEEK:
                        state = TAPE_SEEK;
                        emit progress(0, "seeking", 0, true);
                        _io_seek_blocking(c.arg);
                        iTotal++;
                        emit progress(100, "OK", 0, true);
                        state = TAPE_IDLE;
                        break;
                    case CMD_READ:
                        _total_bytes = c.arg;
                        _remaining_bytes = c.arg;
                        emit progress(0, "reading", _total_bytes - _remaining_bytes, true);
                        state = TAPE_READ;
                        break;
                    case CMD_WRITE:
                        _total_bytes = c.arg;
                        _remaining_bytes = c.arg;
                        emit progress(0, "writing", _total_bytes - _remaining_bytes, true);
                        state = TAPE_WRITE;
                        break;
                    case CMD_SCAN:
                        emit progress(0, "scan", 0, true);
                        if(tapeCatalog != nullptr) {
                            delete tapeCatalog;
                        }
                        tapeCatalog = new TapeCatalog(mediaInfo.BlockSize);
                        state = TAPE_SCAN;
                        break;
                    case CMD_GETPOS:
                        GetPosition();
                        iTotal++;
                        emit change_pos(true);
                        emit progress(100, "pos", 0, true);
                        break;
                    }
                } else {
                    usleep(50000);
                }
            } else if(state == TAPE_READ) {
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
                        usleep(50000);
                        continue;
                    }
                    qRead.enqueue(c);
                    mediaPositionBytes += c.len;
                    emit change_pos();
                    _remaining_bytes -= c.len;
                    emit progress((double)(_total_bytes - _remaining_bytes) / _total_bytes * 100, "reading", _total_bytes - _remaining_bytes);
                } else {
                    iTotal++;
                    emit progress(100, "OK", 0, true);
                    emit change_pos(true);
                    state = TAPE_IDLE;
                    continue;
                }
            } else if(state == TAPE_SCAN) {
                if(_cmd_abort == cmd_abort) {
                    Chunk_t c;
                    c.len = max_chunk_len;
                    c.data = malloc(c.len);
                    if(c.data == nullptr) {
                        usleep(50000);
                        continue;
                    }
                    c.positionBytes = mediaPositionBytes;

                    if(_io_read_blocking(c.data, c.len)) {
                        free(c.data);
                        state = TAPE_ERROR;
                        emit error_signal(QString::number(mediaPositionBytes) + " Tape loading error");
                        usleep(50000);
                        continue;
                    }
                    if(tapeCatalog->search(c.positionBytes, (const char*) c.data, c.len) >= 0) {
                        iTotal++;
                        emit progress(100, "found", 0, true);
                        state = TAPE_IDLE;
                        emit catalog_readed(tapeCatalog);
                        mediaPositionBytes += c.len;
                    } else {
                        mediaPositionBytes += c.len;
                        emit progress((double) (mediaPositionBytes * 100) / mediaInfo.Capacity.QuadPart, "scan", mediaPositionBytes);
                    }
                    emit change_pos();
                    free(c.data);
                } else {
                    _cmd_abort = cmd_abort;
                    delete tapeCatalog;
                    tapeCatalog = nullptr;
                    emit progress((double) (mediaPositionBytes * 100) / mediaInfo.Capacity.QuadPart, "scan aborted", mediaPositionBytes);
                    emit change_pos(true);
                    state = TAPE_IDLE;
                }
            } else if(state == TAPE_WRITE) {
                if(_remaining_bytes > 0) {
                    if(qWrite.isEmpty()) {
                        paused = true;
                    } else if(!paused) {
                        Chunk_t c = qWrite.dequeue();
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
                            emit progress((double)(_total_bytes - _remaining_bytes) / _total_bytes * 100, c.afp, _total_bytes - _remaining_bytes);
                        }
                        free(c.data);
                    }
                } else {
                    iTotal++;
                    paused = true;
                    emit progress(100, "OK", 0, true);
                    emit change_pos(true);
                    state = TAPE_IDLE;
                }
            } else if(state == TAPE_ERROR) {
                cmd_abort++;
                continue;
            } else {
                if(_cmd_abort != cmd_abort) {
                    _cmd_abort = cmd_abort;
                    command.clear();
                }
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

    static const char * StatusString(int status) {
        typedef struct {
            int n;
            const char * s;
        } _t;
        static const _t t[] = {
            {1102L, "ERROR_BEGINNING_OF_MEDIA"},
            {1111L, "ERROR_BUS_RESET"},
            {1107L, "ERROR_DEVICE_NOT_PARTITIONED"},
            {1165L, "ERROR_DEVICE_REQUIRES_CLEANING"},
            {1100L, "ERROR_END_OF_MEDIA"},
            {1101L, "ERROR_FILEMARK_DETECTED"},
            {1106L, "ERROR_INVALID_BLOCK_LENGTH"},
            {1110L, "ERROR_MEDIA_CHANGED"},
            {1104L, "ERROR_NO_DATA_DETECTED"},
            {1112L, "ERROR_NO_MEDIA_IN_DRIVE"},
            {50L, "ERROR_NOT_SUPPORTED"},
            {1105L, "ERROR_PARTITION_FAILURE"},
            {1103L, "ERROR_SETMARK_DETECTED"},
            {1108L, "ERROR_UNABLE_TO_LOCK_MEDIA"},
            {1109L, "ERROR_UNABLE_TO_UNLOAD_MEDIA"},
            {19L, "ERROR_WRITE_PROTECT"},
        };
        for(size_t i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
            if(status == t[i].n)
                return t[i].s;
        }
        return "";
    }

    int Write(void * data, uint32_t len, QString afp) {
        quint64 writeCacheSize = qWriteTotalBytesPut - qWriteTotalBytesGet;
//        emit log(0, QString("Write ") + QString::number(writeCacheSize) + ", " + QString::number(len) + "(" + QString::number(writeCacheSizeMax) + ")");
        if((quint64) writeCacheSize + len > (quint64) writeCacheSizeMax) {
            paused = false;
            return 1;
        }
        Chunk_t c;
        c.data = malloc(len);
        if(c.data == nullptr) {
//            emit log(0, QString("!malloc at ").arg(qWriteBytes, 15, 10, QChar('0')));
            paused = false;
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
    quint64 writeCacheSizeMax = 256ULL * 1024ULL * 1024ULL;

    TAPE_GET_MEDIA_PARAMETERS mediaInfo;
    TAPE_GET_DRIVE_PARAMETERS driveInfo;
    uint64_t mediaPositionBytes = 0;
    TapeCatalog * tapeCatalog = nullptr;
    bool paused = true;

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

    int setTapeDriveParameters(TAPE_SET_DRIVE_PARAMETERS * ptr);
    int setTapeMediaParameters(TAPE_SET_MEDIA_PARAMETERS * ptr);
    bool nullTape = false;

signals:
    void progress1(double percent, quint64 bytes/*, QString str*/);
    /* если в параметрах есть uint64_t, то сигнал не доходит при вызове из другого потока. qt 5.14.2*/
    void progress(double percent, QString str, quint64 bytes, bool force = false);
    void catalog_readed(TapeCatalog * catalog);
    void change_pos(bool force = false);
    void error_signal(QString message);
    void change_cache(quint64 size);
    void log(int level, QString message);
    void status(quint32 tapeStatus);
    void DriveInfoUpdateSignal(void);
    void MediaInfoUpdateSignal(void);

private:
    HANDLE hTape = INVALID_HANDLE_VALUE;
    quint32 tapeStatus = 99999999UL;

    typedef struct {
        void * data;
        uint32_t len;
        uint64_t positionBytes;
        QString afp;
    } Chunk_t;

    QAsyncQueue<Chunk_t> qRead;
    quint64 qReadBytes = 0;

    QAsyncQueue<Chunk_t> qWrite;
    quint64 qWriteBytes = 0;
    quint64 qWriteTotalBytesPut = 0;
    quint64 qWriteTotalBytesGet = 0;

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

    int _getTapeDriveParameters(void);
    int _getTapeMediaParameters(void);

};

#endif // IOTAPE_H
