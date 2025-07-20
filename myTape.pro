QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backbuffer.cpp \
    iodisk.cpp \
    iotape.cpp \
    lib.cpp \
    main.cpp \
    mainwindow.cpp \
    myqbytearray.cpp \
    tapeErase.cpp \
    tapecatalog.cpp \
    worker_writefilelist.cpp

HEADERS += \
    ASYNC_queue.h \
    backbuffer.h \
    iodisk.h \
    iotape.h \
    lib.h \
    mainwindow.h \
    myqbytearray.h \
    tapeErase.h \
    tapecatalog.h \
    worker_writefilelist.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
