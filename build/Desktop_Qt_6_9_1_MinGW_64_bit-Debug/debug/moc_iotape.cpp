/****************************************************************************
** Meta object code from reading C++ file 'iotape.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../iotape.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iotape.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6IOTapeE_t {};
} // unnamed namespace

template <> constexpr inline auto IOTape::qt_create_metaobjectdata<qt_meta_tag_ZN6IOTapeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "IOTape",
        "progress",
        "",
        "iTotal",
        "nTotal",
        "percent",
        "str",
        "uint64_t",
        "bytes",
        "catalog_readed",
        "TapeCatalog*",
        "catalog",
        "change_pos",
        "error_signal",
        "message",
        "change_cache",
        "size",
        "log",
        "level",
        "status",
        "DWORD",
        "tapeStatus"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'progress'
        QtMocHelpers::SignalData<void(int, int, double, QString, uint64_t)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Double, 5 }, { QMetaType::QString, 6 },
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'progress'
        QtMocHelpers::SignalData<void(int, int, double, QString)>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Double, 5 }, { QMetaType::QString, 6 },
        }}),
        // Signal 'progress'
        QtMocHelpers::SignalData<void(int, int, double)>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Double, 5 },
        }}),
        // Signal 'catalog_readed'
        QtMocHelpers::SignalData<void(TapeCatalog *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Signal 'change_pos'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'error_signal'
        QtMocHelpers::SignalData<void(QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Signal 'change_cache'
        QtMocHelpers::SignalData<void(uint64_t)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 16 },
        }}),
        // Signal 'log'
        QtMocHelpers::SignalData<void(int, QString)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 14 },
        }}),
        // Signal 'status'
        QtMocHelpers::SignalData<void(DWORD)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<IOTape, qt_meta_tag_ZN6IOTapeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject IOTape::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6IOTapeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6IOTapeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6IOTapeE_t>.metaTypes,
    nullptr
} };

void IOTape::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<IOTape *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<uint64_t>>(_a[5]))); break;
        case 1: _t->progress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 2: _t->progress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 3: _t->catalog_readed((*reinterpret_cast< std::add_pointer_t<TapeCatalog*>>(_a[1]))); break;
        case 4: _t->change_pos(); break;
        case 5: _t->error_signal((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->change_cache((*reinterpret_cast< std::add_pointer_t<uint64_t>>(_a[1]))); break;
        case 7: _t->log((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->status((*reinterpret_cast< std::add_pointer_t<DWORD>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(int , int , double , QString , uint64_t )>(_a, &IOTape::progress, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(TapeCatalog * )>(_a, &IOTape::catalog_readed, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)()>(_a, &IOTape::change_pos, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(QString )>(_a, &IOTape::error_signal, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(uint64_t )>(_a, &IOTape::change_cache, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(int , QString )>(_a, &IOTape::log, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (IOTape::*)(DWORD )>(_a, &IOTape::status, 8))
            return;
    }
}

const QMetaObject *IOTape::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IOTape::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6IOTapeE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int IOTape::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void IOTape::progress(int _t1, int _t2, double _t3, QString _t4, uint64_t _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 3
void IOTape::catalog_readed(TapeCatalog * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void IOTape::change_pos()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void IOTape::error_signal(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void IOTape::change_cache(uint64_t _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void IOTape::log(int _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void IOTape::status(DWORD _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}
QT_WARNING_POP
