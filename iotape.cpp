#include <QFile>
#include "iotape.h"

static HANDLE _cfw(const char * s) {
#ifdef  WIN64
    const WCHAR *portNameW;
    int nChars = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
    portNameW = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, s, -1, (LPWSTR)portNameW, nChars);
    HANDLE handle = CreateFile(portNameW, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
    delete[] portNameW;
    return handle;
#else   /*WIN64*/
    return CreateFile(s, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#endif  /*WIN64*/
}

int IOTape::Open(const char * device, int buffLen) {
    int err = -1;
    Close();
    memset(&mediaInfo, 0, sizeof(mediaInfo));
    memset(&driveInfo, 0, sizeof(driveInfo));
    do {
        if(buffLen < 1)
            buffLen = 1;
        writeCacheSizeMax = buffLen * 1024 * 1024;
        if(strcmp(device, "\\\\.\\null")) {
            nullTape = false;
            hTape = _cfw(device);
            if(hTape == INVALID_HANDLE_VALUE)
                break;

            DWORD lpdwSize;
            lpdwSize = sizeof(driveInfo);
            GetTapeParameters(hTape, GET_TAPE_DRIVE_INFORMATION, &lpdwSize, &driveInfo);
            lpdwSize = sizeof(mediaInfo);
            GetTapeParameters(hTape, GET_TAPE_MEDIA_INFORMATION, &lpdwSize, &mediaInfo);

            if(mediaInfo.BlockSize == 0) {
                mediaInfo.BlockSize = driveInfo.DefaultBlockSize;
                TAPE_SET_MEDIA_PARAMETERS mediaSet;
                mediaSet.BlockSize = mediaInfo.BlockSize;
                SetTapeParameters(hTape, SET_TAPE_MEDIA_INFORMATION, &mediaSet);
            }

            GetPosition();
            tapeStatus = GetTapeStatus(hTape);
            emit status(tapeStatus);

        } else {
            nullTape = true;
            mediaInfo.BlockSize = 512;
            mediaInfo.Capacity.HighPart = 256;
            mediaInfo.Capacity.LowPart = 0;
            mediaInfo.PartitionCount = 1;
            mediaInfo.Remaining.HighPart = mediaInfo.Capacity.HighPart;
            mediaInfo.Remaining.LowPart = mediaInfo.Capacity.LowPart;
            mediaInfo.WriteProtected = false;

            mediaPositionBytes = 0;
        }
        err = 0;
    } while(0);
    return err;
}

void IOTape::Close(void) {
    if(hTape != INVALID_HANDLE_VALUE) {
        CloseHandle(hTape);
        hTape = INVALID_HANDLE_VALUE;
    }
    nullTape = false;
}

int IOTape::_io_read_blocking(void * dest, uint32_t len) {
#ifdef  TAPE_EMULATION_FILE
    QFile f(TAPE_EMULATION_FILE);
    if(!f.open(QFile::OpenModeFlag::ReadOnly | QFile::OpenModeFlag::Unbuffered))
        return -1;
    f.seek(mediaPositionBytes);
    qint64 n1 = f.read((char*) dest, len);
    if(n1 <= 0) {
        f.close();
        return -1;
    }
    if(n1 < len) {
        memset((char *) dest + n1, 0, len - n1);
    }
    f.close();
#else   /*TAPE_EMULATION_FILE*/
    DWORD n1 = 0;
    BOOL result = ReadFile(hTape, dest, len, &n1, NULL);
    if(!result || n1 != len) {
        return -1;
    }
#endif  /*TAPE_EMULATION_FILE*/
    return 0;
}

int IOTape::_io_write_blocking(void * src, uint32_t len) {
#ifdef  TAPE_EMULATION_FILE
    QFile f(TAPE_EMULATION_FILE);
    if(!f.open(QFile::OpenModeFlag::ReadWrite | QFile::OpenModeFlag::Unbuffered))
        return -1;
    f.seek(mediaPositionBytes);
    f.write((const char*) src, len);
    f.close();
#else   /*TAPE_EMULATION_FILE*/
    if(!nullTape) {
        DWORD n1 = 0;
        BOOL result = WriteFile(hTape, src, len, &n1, NULL);
        if(!result || n1 != len) {
            return -1;
        }
    }
#endif  /*TAPE_EMULATION_FILE*/
    return 0;
}

int IOTape::_io_seek_blocking(uint64_t pos) {
    pos /= this->mediaInfo.BlockSize;
#ifdef  TAPE_EMULATION_FILE
#else   /*TAPE_EMULATION_FILE*/
    if(!nullTape) {
        DWORD dwOffsetLow = (DWORD) pos;
        DWORD dwOffsetHigh = (DWORD) (pos >> 32);

        DWORD result = SetTapePosition(hTape, TAPE_ABSOLUTE_BLOCK, 0, dwOffsetLow, dwOffsetHigh, FALSE);
        if(result != NO_ERROR) {
            return -1;
        }
    }
#endif  /*TAPE_EMULATION_FILE*/
    mediaPositionBytes = pos * this->mediaInfo.BlockSize;
    emit change_pos();
    return 0;
}
