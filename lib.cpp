#include <windows.h>
#include <string>
#include <QString>
#include "lib.h"

quint64 roundUp(quint64 value, quint64 granularity){
    quint64 p = (value + granularity - 1)/ granularity;
    return p * granularity;
}

QString GetLastErrorAsString(void) {
    // Get the error code
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0) {
        return "No error message has been recorded.";
    }

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL,
                                 errorMessageID,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 (LPWSTR)&messageBuffer,
                                 0,
                                 NULL);

    QString result = QString::fromWCharArray(messageBuffer, size);
    // Free the buffer allocated by FormatMessage
    LocalFree(messageBuffer);

    return result + " (" + QString::number(errorMessageID) + ")";
}
