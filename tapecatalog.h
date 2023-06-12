#ifndef TAPECATALOG_H
#define TAPECATALOG_H

#include <QList>
#include <QFileInfo>
#include <QFile>
#include <QByteArray>
#include "myqbytearray.h"

class TapeCatalog
{
public:
    typedef enum {
        VERSION_0_0,
        VERSION_0_1,
    } Version_e;

    TapeCatalog(uint32_t blockSize, QList<QFileInfo> files = QList<QFileInfo>()) {
        this->blockSize = blockSize;
        this->filesOnDisk = files;
    }

    TapeCatalog append(QFileInfo file) {
        filesOnDisk.append(file);
        return *this;
    }

    TapeCatalog append(QList<QFileInfo> files) {
        files.append(files);
        return *this;
    }

    TapeCatalog clear(void) {
        filesOnDisk.clear();
        return *this;
    }

    /*
     * lsbf
     * +0: signature
     * +x: uint32_t catalog.length()
     * {
     * +n+0: uint16_t file.absoluteFilePath().toUtf8().length()
     * +n+2: file.absoluteFilePath().toUtf8()
     * +n+2+l: uint64_t file.size()
     * +n+2+l+8: uint64_t offset;
     * }
     * +z: uint16_t 0
     *
     *
    */

    /*
     * lsbf
     * +0: signature1
     * +x: uint32_t catalog.length()
     * {
     * +n+0: uint16_t file.absoluteFilePath().toUtf8().length()
     * +n+2: file.absoluteFilePath().toUtf8()
     * +n+2+l: uint64_t file.size()
     * +n+2+l+8: uint64_t offset;
     * +n+2+l+8+8: uint64_t datetime_ms;
     * +n+2+l+8+8+8: uint64_t attr;
     *
     * }
     * +z: uint16_t 0
     *
     *
    */

    QByteArray serialize(Version_e v = VERSION_0_1) {
        this->ver = v;
        myQByteArray catalog;
        if(ver == VERSION_0_0) {
            catalog.append(signature);
        } else if(ver == VERSION_0_1) {
            catalog.append(signature1);
        }
        qsizetype catalogLenPos = catalog.length();
        catalog.append_lsbf((uint32_t) 0);

        typedef struct {
            uint64_t offset;
            uint32_t offsetPos;
        } a_t;
        QList<a_t> fileOffsets;

        uint64_t baseOffset = 0;
        for(int i = 0; i < filesOnDisk.length(); i++) {
            QByteArray fileNamePath = filesOnDisk.value(i).absoluteFilePath().toUtf8();

            uint64_t fileSize = 0;
            a_t a;
            if(ver == VERSION_0_0) {
                fileSize = filesOnDisk.value(i).size();

                catalog.append_lsbf((uint16_t) fileNamePath.length());
                catalog.append(fileNamePath);
                catalog.append_lsbf(fileSize);
                a.offsetPos = catalog.length();
                a.offset = baseOffset;
                catalog.append_lsbf(baseOffset);

            } else if(ver == VERSION_0_1) {
                uint64_t fileDateBirth = filesOnDisk.value(i).birthTime().toMSecsSinceEpoch();
                uint64_t fileAttr = 0;
                if(filesOnDisk.value(i).isDir()) {
                    fileAttr |= 1;
                } else {
                    fileSize = filesOnDisk.value(i).size();
                }
                if(filesOnDisk.value(i).isHidden())
                    fileAttr |= 2;

                catalog.append_lsbf((uint16_t) fileNamePath.length());
                catalog.append(fileNamePath);
                catalog.append_lsbf(fileSize);
                a.offsetPos = catalog.length();
                a.offset = baseOffset;
                catalog.append_lsbf(baseOffset);

                catalog.append_lsbf(fileDateBirth);
                catalog.append_lsbf(fileAttr);
            }

            baseOffset += fileSize + blockSize - 1;
            baseOffset &= ~((uint64_t) blockSize - 1);
            fileOffsets.append(a);
        }
        catalog.append_lsbf((uint16_t) 0);

        catalog.write_lsbf(catalogLenPos, (uint32_t) catalog.length());
        uint64_t catalogLenRounded = catalog.length() + blockSize - 1;
        catalogLenRounded &= ~((uint64_t) blockSize - 1);

        for(int i = 0; i < fileOffsets.length(); i++) {
            catalog.write_lsbf(fileOffsets.value(i).offsetPos, fileOffsets.value(i).offset + catalogLenRounded);
        }
        totalSize = baseOffset + catalogLenRounded;
        return catalog.toQByteArray();
    }

    int64_t search(uint64_t offset, const char * data, uint32_t len) {
        uint32_t pos = 0;

        while(pos < len) {
            if(searchState == 0) {
                searchBuff.clear();
                totalSize = 0;
                pos &= ~(blockSize - 1);
                bool signFound = false;
                qsizetype signLen = 0;
                qsizetype searchCatalogMinLen = 0;
                if(!memcmp(data + pos, signature.data(), signature.length())) {
                    signFound = true;
                    ver = VERSION_0_0;
                    signLen = signature.length();
                    searchCatalogMinLen = signLen + 4 + 2 + 1 + 8 + 8 + 2;
                } else if(!memcmp(data + pos, signature1.data(), signature1.length())) {
                    signFound = true;
                    ver = VERSION_0_1;
                    signLen = signature1.length();
                    searchCatalogMinLen = signLen + 4 + 2 + 1 + 8 + 8 + 8 + 8 + 2;
                }
                if(!signFound) {
                    pos += blockSize;
                    continue;
                }

                offsetOnTape = offset + pos;
                filesOnTape.clear();
                searchBuff.append(data + pos, len - pos);
                searchCatalogLen = searchBuff.read_u32lsbf(signLen);
                if(searchCatalogLen < searchCatalogMinLen) {
                    pos += blockSize;
                    continue;
                }
                if(searchCatalogLen > searchBuff.length()) {
                    searchState = 1;
                    break;
                }
                searchState = 2;
                continue;
            }
            if(searchState == 1) {
                uint32_t l = len;
                if(l > searchCatalogLen - searchBuff.length())
                    l = searchCatalogLen - searchBuff.length();
                searchBuff.append(data, l);
                if(searchCatalogLen > searchBuff.length()) {
                    break;
                }
                searchState = 2;
                continue;
            }
            if(searchState == 2) {
                uint32_t c_pos = signature.length() + 4;
                while(c_pos < searchBuff.length()) {
                    fileOnTape_t f;
                    f.fileAttr = 0;
                    f.fileDateBirth = 0;
                    if(ver == VERSION_0_0) {
                        uint16_t c_name_len = searchBuff.read_u16lsbf(c_pos);
                        if(c_name_len == 0)
                            break;
                        c_pos += 2;
                        f.fileNamePath = QString::fromUtf8(searchBuff.data() + c_pos, c_name_len);
                        c_pos += c_name_len;
                        f.fileSize = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                        f.offset = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                    } else if(ver == VERSION_0_1) {
                        uint16_t c_name_len = searchBuff.read_u16lsbf(c_pos);
                        if(c_name_len == 0)
                            break;
                        c_pos += 2;
                        f.fileNamePath = QString::fromUtf8(searchBuff.data() + c_pos, c_name_len);
                        c_pos += c_name_len;
                        f.fileSize = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                        f.offset = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                        f.fileDateBirth = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                        f.fileAttr = searchBuff.read_u64lsbf(c_pos);
                        c_pos += 8;
                    }

                    uint64_t fileSizeRounded = (uint64_t)(f.fileSize + blockSize - 1) & ~(uint64_t)(blockSize - 1);
                    if(totalSize < f.offset + fileSizeRounded) {
                        totalSize = f.offset + fileSizeRounded;
                    }

                    if((f.fileAttr & 1) == 0) {
                        filesOnTape.append(f);
                    }
                }
                return (int64_t) offsetOnTape;
            }
        }
        return -1;
    }

    enum ExportFileFormat {
        CSV,
    };

    int export_to_file(QFile f, ExportFileFormat format = CSV) {
        if(format == CSV) {
            if(!f.open(QFile::WriteOnly)) {
                return -2;
            }

            f.write(("offsetOnTape," + QString::number(offsetOnTape)).toLatin1().append('\n'));
            f.write(("totalSize," + QString::number(totalSize)).toLatin1().append('\n'));
            f.write(("files," + QString::number(filesOnTape.length())).toLatin1().append('\n'));
            f.write(QString("Name,Size,Offset,").toLatin1().append('\n'));

            for(int i = 0; i < filesOnTape.length(); i++) {
                auto res = filesOnTape.value(i);
                QString a = "\"" + res.fileNamePath + QString("\",") + QString::number(res.fileSize) + QString(",") + QString::number(res.offset);
                QByteArray b = a.toUtf8();
                f.write(b.append('\n'));
            }
            f.close();
        } else {
            return -1;
        }
        return 0;
    }

    typedef struct {
        QString fileNamePath;
        uint64_t fileSize;
        uint64_t offset;
        uint64_t fileDateBirth;
        uint64_t fileAttr;
        bool skip;
    } fileOnTape_t;
    QList<fileOnTape_t> filesOnTape;
    uint64_t offsetOnTape = 0;
    uint64_t totalSize = 0;
    Version_e ver = VERSION_0_0;

private:
    uint64_t blockSize;
    QList<QFileInfo> filesOnDisk;
    const QByteArray signature = QString("my!tape!catalog!0.0").toLatin1();
    const QByteArray signature1 = QString("my!tape!catalog!0.1").toLatin1();

    /*search*/
    int searchState = 0;
    uint32_t searchCatalogLen = 0;
    myQByteArray searchBuff;
};

#endif // TAPECATALOG_H
