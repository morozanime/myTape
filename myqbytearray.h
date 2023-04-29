#ifndef MYQBYTEARRAY_H
#define MYQBYTEARRAY_H

#include <QByteArray>

class myQByteArray : public QByteArray
{
public:
    myQByteArray append_lsbf(uint16_t value) {
        this->append(static_cast<char>((value >> 0) & 0xFF));
        this->append(static_cast<char>((value >> 8) & 0xFF));
        return *this;
    }

    myQByteArray append_lsbf(uint32_t value) {
        this->append_lsbf(static_cast<uint16_t>((value >> 0) & 0xFFFF));
        this->append_lsbf(static_cast<uint16_t>((value >> 16) & 0xFFFF));
        return *this;
    }

    myQByteArray append_lsbf(uint64_t value) {
        this->append_lsbf(static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
        this->append_lsbf(static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
        return *this;
    }

    myQByteArray write_lsbf(qsizetype offset, uint16_t value) {
        if(this->length() < offset + 2) {
            this->append(offset + 2 - this->length(), '\0');
        }
        uint8_t a[2] = {
            static_cast<uint8_t>((value >> 0) & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
        };
        this->replace(offset, 2, (const char*) a, 2);
        return *this;
    }

    myQByteArray write_lsbf(qsizetype offset, uint32_t value) {
        if(this->length() < offset + 4) {
            this->append(offset + 4 - this->length(), '\0');
        }
        uint8_t a[4] = {
            static_cast<uint8_t>((value >> 0) & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
            static_cast<uint8_t>((value >> 16) & 0xFF),
            static_cast<uint8_t>((value >> 24) & 0xFF),
        };
        this->replace(offset, 4, (const char*) a, 4);
        return *this;
    }

    myQByteArray write_lsbf(qsizetype offset, uint64_t value) {
        if(this->length() < offset + 8) {
            this->append(offset + 8 - this->length(), '\0');
        }
        uint8_t a[8] = {
            static_cast<uint8_t>((value >> 0) & 0xFF),
            static_cast<uint8_t>((value >> 8) & 0xFF),
            static_cast<uint8_t>((value >> 16) & 0xFF),
            static_cast<uint8_t>((value >> 24) & 0xFF),
            static_cast<uint8_t>((value >> 32) & 0xFF),
            static_cast<uint8_t>((value >> 40) & 0xFF),
            static_cast<uint8_t>((value >> 48) & 0xFF),
            static_cast<uint8_t>((value >> 56) & 0xFF),
        };
        this->replace(offset, 8, (const char*) a, 8);
        return *this;
    }

    uint16_t read_u16lsbf(qsizetype offset) {
        return (uint16_t) read_ulsbf(offset, 2);
    }

    uint32_t read_u32lsbf(qsizetype offset) {
        return (uint32_t) read_ulsbf(offset, 4);
    }

    uint64_t read_u64lsbf(qsizetype offset) {
        return (uint64_t) read_ulsbf(offset, 8);
    }

    uint64_t read_ulsbf(qsizetype offset, qsizetype len) {
        if(len == 0 || offset + len > this->length())
            return 0;
        uint64_t value = 0;
        do {
            len--;
            value <<= 8;
            value |= static_cast<uint8_t>(this->at(offset + len));
        } while(len > 0);
        return value;
    }

    QByteArray toQByteArray(void) {
        QByteArray a(this->data(), this->length());
        return a;
    }
};

#endif // MYQBYTEARRAY_H
