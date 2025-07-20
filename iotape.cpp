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
    return CreateFileA(s, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#endif  /*WIN64*/
}

int IOTape::_getTapeDriveParameters(void) {
    int err = -1;
    do {
        emit log(0, "--- GET_TAPE_DRIVE_INFORMATION");
        DWORD lpdwSize;
        lpdwSize = sizeof(driveInfo);
        DWORD driveInfoResult = GetTapeParameters(hTape, GET_TAPE_DRIVE_INFORMATION, &lpdwSize, &driveInfo);
        if(driveInfoResult != NO_ERROR) {
            tapeStatus = driveInfoResult;
            emit status(tapeStatus);
            emit log(0, "ERROR " + QString::number(tapeStatus));
            break;
        }
        emit log(0, "ECC " + QString::number(driveInfo.ECC));
        emit log(0, "Compression " + QString::number(driveInfo.Compression));
        emit log(0, "DataPadding " + QString::number(driveInfo.DataPadding));
        emit log(0, "ReportSetmarks " + QString::number(driveInfo.ReportSetmarks));
        emit log(0, "DefaultBlockSize " + QString::number(driveInfo.DefaultBlockSize));
        emit log(0, "MaximumBlockSize " + QString::number(driveInfo.MaximumBlockSize));
        emit log(0, "MinimumBlockSize " + QString::number(driveInfo.MinimumBlockSize));
        emit log(0, "MaximumPartitionCount " + QString::number(driveInfo.MaximumPartitionCount));
        emit log(0, "FeaturesLow " + QString::number((quint64) driveInfo.FeaturesLow, 16));
        emit log(0, "FeaturesHigh " + QString::number((quint64) driveInfo.FeaturesHigh, 16));
        emit log(0, "EOTWarningZoneSize " + QString::number(driveInfo.EOTWarningZoneSize));

        emit DriveInfoUpdateSignal();
        err = 0;
    } while(0);
    return err;
}

int IOTape::_getTapeMediaParameters(void) {
    int err = -1;
    do {
        emit log(0, "--- GET_TAPE_MEDIA_INFORMATION");
        DWORD lpdwSize;
        lpdwSize = sizeof(mediaInfo);
        DWORD mediaInfoResult = GetTapeParameters(hTape, GET_TAPE_MEDIA_INFORMATION, &lpdwSize, &mediaInfo);
        if(mediaInfoResult != NO_ERROR) {
            tapeStatus = mediaInfoResult;
            emit status(tapeStatus);
            emit log(0, "ERROR " + QString::number(tapeStatus));
            break;
        }
        if(mediaInfo.Capacity.QuadPart == 0) {
            mediaInfo.Capacity.QuadPart = mediaInfo.Remaining.QuadPart;
        }

        emit log(0, "Capacity " + QString::number(mediaInfo.Capacity.QuadPart));
        emit log(0, "Remaining " + QString::number(mediaInfo.Remaining.QuadPart));
        emit log(0, "BlockSize " + QString::number(mediaInfo.BlockSize));
        emit log(0, "PartitionCount " + QString::number(mediaInfo.PartitionCount));
        emit log(0, "WriteProtected " + QString::number(mediaInfo.WriteProtected));

        emit MediaInfoUpdateSignal();
        err = 0;
    } while(0);
    return err;
}

int IOTape::setTapeDriveParameters(TAPE_SET_DRIVE_PARAMETERS * ptr){
    int err = -1;
    do {
        emit log(0, "--- SET_TAPE_DRIVE_PARAMETERS");
        DWORD result = SetTapeParameters(hTape, SET_TAPE_DRIVE_INFORMATION, ptr);
        if(result != NO_ERROR) {
            tapeStatus = result;
            emit status(tapeStatus);
            emit log(0, "ERROR " + QString::number(tapeStatus));
            break;
        }
        _getTapeDriveParameters();
        err = 0;
    } while(0);
    return err;
}

int IOTape::setTapeMediaParameters(TAPE_SET_MEDIA_PARAMETERS * ptr){
    int err = -1;
    do {
        emit log(0, "--- SET_TAPE_MEDIA_PARAMETERS");
        DWORD result = SetTapeParameters(hTape, SET_TAPE_MEDIA_INFORMATION, ptr);
        if(result != NO_ERROR) {
            tapeStatus = result;
            emit status(tapeStatus);
            emit log(0, "ERROR " + QString::number(tapeStatus));
            break;
        }
        _getTapeMediaParameters();
        err = 0;
    } while(0);
    return err;
}

int IOTape::Open(const char * device, int buffLen) {
    int err = -1;
    Close();
    memset(&mediaInfo, 0, sizeof(mediaInfo));
    memset(&driveInfo, 0, sizeof(driveInfo));
    do {
        if(buffLen < 1)
            buffLen = 1;
        writeCacheSizeMax = (quint64) buffLen * 1024ULL * 1024ULL;
        emit log(0, "--- Open '" + QString(device) + "'");
        if(strcmp(device, "\\\\.\\null")) {
            nullTape = false;
            hTape = _cfw(device);
            if(hTape == INVALID_HANDLE_VALUE) {
                emit log(0, "Open error:" + QString::number(GetLastError()));
                break;
            }

            if(_getTapeDriveParameters()) {
                break;
            }

            if(_getTapeMediaParameters()) {
                break;
            }

            if(mediaInfo.BlockSize == 0) {
                mediaInfo.BlockSize = driveInfo.DefaultBlockSize;
                TAPE_SET_MEDIA_PARAMETERS mediaSet;
                mediaSet.BlockSize = mediaInfo.BlockSize;
                if(setTapeMediaParameters(&mediaSet)) {
                    break;
                }
                if(_getTapeMediaParameters()) {
                    break;
                }
            }

            GetPosition(&mediaPositionBytes);
            tapeStatus = GetTapeStatus(hTape);
            emit status(tapeStatus);

        } else {
            nullTape = true;
            hTape = INVALID_HANDLE_VALUE;

            driveInfo.DefaultBlockSize = 512;
            driveInfo.MinimumBlockSize = 256;
            driveInfo.MaximumBlockSize = 8 * 1024 * 1024;

            mediaInfo.BlockSize = 512;
            mediaInfo.Capacity.HighPart = 256;
            mediaInfo.Capacity.LowPart = 0;
            mediaInfo.PartitionCount = 1;
            mediaInfo.Remaining.HighPart = mediaInfo.Capacity.HighPart;
            mediaInfo.Remaining.LowPart = mediaInfo.Capacity.LowPart;
            mediaInfo.WriteProtected = false;

            mediaPositionBytes = 0;

            tapeStatus = 0;

            emit DriveInfoUpdateSignal();
            emit MediaInfoUpdateSignal();
            emit status(tapeStatus);
        }
        err = 0;
    } while(0);
    if(err != 0) {
        if(!nullTape) {
            Close();
        }
    }
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

int IOTape::_io_seek_blocking(quint64 pos) {
    pos /= this->mediaInfo.BlockSize;
#ifdef  TAPE_EMULATION_FILE
#else   /*TAPE_EMULATION_FILE*/
    if(!nullTape) {
        DWORD dwOffsetLow = (DWORD) pos;
        DWORD dwOffsetHigh = (DWORD) (pos >> 32ULL);

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
