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

    QByteArray serialize(void) {
        myQByteArray catalog;
        catalog.append(signature);
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
            uint64_t fileSize = filesOnDisk.value(i).size();
            a_t a;
            catalog.append_lsbf((uint16_t) fileNamePath.length());
            catalog.append(fileNamePath);
            catalog.append_lsbf(fileSize);
            a.offsetPos = catalog.length();
            a.offset = baseOffset;
            catalog.append_lsbf(baseOffset);
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
                if(memcmp(data + pos, signature.data(), signature.length())) {
                    pos += blockSize;
                    continue;
                }
                offsetOnTape = offset + pos;
                filesOnTape.clear();
                searchBuff.append(data + pos, len - pos);
                searchCatalogLen = searchBuff.read_u32lsbf(signature.length());
                if(searchCatalogLen < signature.length() + 4 + 2 + 1 + 8 + 8 + 2) {
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

                    uint64_t fileSizeRounded = (uint64_t)(f.fileSize + blockSize - 1) & ~(uint64_t)(blockSize - 1);
                    if(totalSize < f.offset + fileSizeRounded) {
                        totalSize = f.offset + fileSizeRounded;
                    }

                    filesOnTape.append(f);
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
        bool skip;
    } fileOnTape_t;
    QList<fileOnTape_t> filesOnTape;
    uint64_t offsetOnTape = 0;
    uint64_t totalSize = 0;

private:
    uint64_t blockSize;
    QList<QFileInfo> filesOnDisk;
    const QByteArray signature = QString("my!tape!catalog!0.0").toLatin1();

    /*search*/
    int searchState = 0;
    uint32_t searchCatalogLen = 0;
    myQByteArray searchBuff;
};

#endif // TAPECATALOG_H
