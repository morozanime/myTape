/****************************************************************************
** Meta object code from reading C++ file 'worker_writefilelist.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../worker_writefilelist.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'worker_writefilelist.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN20Worker_WriteFileListE_t {};
} // unnamed namespace

template <> constexpr inline auto Worker_WriteFileList::qt_create_metaobjectdata<qt_meta_tag_ZN20Worker_WriteFileListE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Worker_WriteFileList",
        "AddFile",
        "",
        "path",
        "size",
        "CatalogReady",
        "total_size",
        "Clear",
        "progress",
        "iTotal",
        "nTotal",
        "percent",
        "str",
        "uint64_t",
        "bytes"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'AddFile'
        QtMocHelpers::SignalData<void(QString, qint64)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::LongLong, 4 },
        }}),
        // Signal 'CatalogReady'
        QtMocHelpers::SignalData<void(qint64)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 6 },
        }}),
        // Signal 'Clear'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'progress'
        QtMocHelpers::SignalData<void(int, int, double, QString, uint64_t)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 10 }, { QMetaType::Double, 11 }, { QMetaType::QString, 12 },
            { 0x80000000 | 13, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Worker_WriteFileList, qt_meta_tag_ZN20Worker_WriteFileListE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Worker_WriteFileList::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Worker_WriteFileListE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Worker_WriteFileListE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20Worker_WriteFileListE_t>.metaTypes,
    nullptr
} };

void Worker_WriteFileList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Worker_WriteFileList *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->AddFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 1: _t->CatalogReady((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 2: _t->Clear(); break;
        case 3: _t->progress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<uint64_t>>(_a[5]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Worker_WriteFileList::*)(QString , qint64 )>(_a, &Worker_WriteFileList::AddFile, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Worker_WriteFileList::*)(qint64 )>(_a, &Worker_WriteFileList::CatalogReady, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Worker_WriteFileList::*)()>(_a, &Worker_WriteFileList::Clear, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Worker_WriteFileList::*)(int , int , double , QString , uint64_t )>(_a, &Worker_WriteFileList::progress, 3))
            return;
    }
}

const QMetaObject *Worker_WriteFileList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Worker_WriteFileList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20Worker_WriteFileListE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Worker_WriteFileList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Worker_WriteFileList::AddFile(QString _t1, qint64 _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Worker_WriteFileList::CatalogReady(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Worker_WriteFileList::Clear()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Worker_WriteFileList::progress(int _t1, int _t2, double _t3, QString _t4, uint64_t _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4, _t5);
}
QT_WARNING_POP
