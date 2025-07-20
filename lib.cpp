#include "lib.h"

quint64 roundUp(quint64 value, quint64 granularity){
    quint64 p = (value + granularity - 1)/ granularity;
    return p * granularity;
}
