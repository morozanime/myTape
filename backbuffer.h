#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <QtGlobal>
#include <string.h>

class backBuffer
{
public:
    backBuffer(quint64 size)
    {
        _data = (quint8*)malloc(size);
        if(_data == nullptr) {
            throw("Memory allocation failed");
        }
        _pos = 0;
        _len = 0;
        _size = size;
    }
    ~backBuffer() {
        free(_data);
    }

    void put(void * data, quint64 len) {
        if(len == 0)
            return;
        quint64 pos = 0;
        if(len > _size) {
            pos = len - _size;
            len = _size;
        }
        quint64 part1 = len;
        quint64 part2 = 0;
        if(part1 > _size - _pos) {
            part1 = _size - _pos;
            part2 = len - part1;
        }
        memcpy((uint8_t *)_data + _pos, (uint8_t *)data + pos, part1);
        _pos += part1;
        if(_pos >= _size)
            _pos = 0;
        pos += part1;
        memcpy((uint8_t *)_data + _pos, (uint8_t *)data + pos, part2);
        _pos += part2;
        _len += len;
        if(_len > _size)
            _len = _size;
    }

    quint64 get(void ** out_data1, void ** out_data2, quint64 * out_part1, quint64 len, quint64 pos = 0) {
        if(pos >= _len)
            return 0;
        if(len + pos > _len) {
            len = _len - pos;
        }
        if(pos == 0) {
            pos = _pos;
        } else if(pos < _pos) {
            pos = _pos - pos;
        } else {
            pos = _size - (pos - _pos);
        }
        if(len > pos) {
            *out_part1 = len - pos;
            *out_data1 = (uint8_t *)_data + _size - (len - pos);
            *out_data2 = _data;
        } else {
            *out_part1 = len;
            *out_data1 = (uint8_t *)_data + pos - len;
        }
        return len;
    }

    void clear(void) {
        _pos = 0;
        _len = 0;
    }

    quint64 len(void) {
        return _len;
    }

private:
    quint64 _pos;
    quint64 _len;
    quint64 _size;
    void * _data;
};

#endif // BACKBUFFER_H
