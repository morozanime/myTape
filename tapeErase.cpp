
#include <windows.h>
#include <winnt.h>
//#include <ntddtape.h>
#include "tapeErase.h"

#define IOCTL_TAPE_BASE                   FILE_DEVICE_TAPE

#define IOCTL_TAPE_CHECK_VERIFY         CTL_CODE(IOCTL_TAPE_BASE, 0x0200, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_CREATE_PARTITION     CTL_CODE(IOCTL_TAPE_BASE, 0x000a, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_ERASE                CTL_CODE(IOCTL_TAPE_BASE, 0x0000, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_FIND_NEW_DEVICES     CTL_CODE(IOCTL_DISK_BASE, 0x0206, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_DRIVE_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0005, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_MEDIA_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0007, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_POSITION         CTL_CODE(IOCTL_TAPE_BASE, 0x0003, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_STATUS           CTL_CODE(IOCTL_TAPE_BASE, 0x0009, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_TAPE_PREPARE              CTL_CODE(IOCTL_TAPE_BASE, 0x0001, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_DRIVE_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0006, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_SET_MEDIA_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0008, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_POSITION         CTL_CODE(IOCTL_TAPE_BASE, 0x0004, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_WRITE_MARKS          CTL_CODE(IOCTL_TAPE_BASE, 0x0002, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

bool tapeErase(HANDLE hTape, bool type) {
    DWORD bytesReturned;
    TAPE_ERASE tapeErase;
    tapeErase.Type = type ? TAPE_ERASE_LONG : TAPE_ERASE_SHORT;
    tapeErase.Immediate = false;
    return DeviceIoControl(
            hTape,                     // Handle to the tape device
            IOCTL_TAPE_ERASE,            // Control code for tape erase
            &tapeErase,                 // Input buffer (TAPE_ERASE structure)
            sizeof(TAPE_ERASE),          // Input buffer size
            NULL,                       // Output buffer (not needed for this operation)
            0,                          // Output buffer size
            &bytesReturned,             // Bytes returned (not needed for this operation)
            NULL                        // Overlapped structure (not needed for this operation)
        );
}

#define IOCTL_TAPE_EJECT_MEDIA  0x1f4808

bool tapeEject(HANDLE hTape) {
    DWORD bytesReturned;
    OVERLAPPED Overlapped;
    DWORD ioCtrl = IOCTL_TAPE_EJECT_MEDIA;
    return DeviceIoControl(
            hTape,                      // Handle to the tape device
            ioCtrl,  //
            NULL,                       // Input buffer
            0,                          // Input buffer size
            NULL,                       // Output buffer (not needed for this operation)
            0,                          // Output buffer size
            &bytesReturned,             // Bytes returned (not needed for this operation)
            &Overlapped                 // Overlapped structure (not needed for this operation)
        );
}
