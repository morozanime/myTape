/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "progress",
        "",
        "i",
        "n",
        "percent",
        "str",
        "uint64_t",
        "bytes",
        "catalog_readed",
        "TapeCatalog*",
        "catalog",
        "change_pos",
        "error_message",
        "message",
        "change_cache",
        "size",
        "log",
        "level",
        "worker_writefilelist_AddFile",
        "path",
        "worker_writefilelist_CatalogReady",
        "total_size",
        "worker_writefilelist_Clear",
        "tapeStatus",
        "DWORD",
        "st",
        "on_pushButtonWriteAddFile_clicked",
        "on_pushButtonWriteClear_clicked",
        "on_pushButtonWriteWrite_clicked",
        "on_pushButtonWriteAddDir_clicked",
        "on_pushButtonWriteAbort_clicked",
        "on_pushButtonOpen_clicked",
        "on_pushButtonSeek_clicked",
        "on_pushButtonGetPos_clicked",
        "on_pushButtonRead_clicked",
        "on_pushButtonScan_clicked",
        "on_pushButtonRestoreAll_clicked",
        "on_pushButtonSeekFirstFree_clicked",
        "on_pushButtonNext_clicked",
        "on_pushButtonExport_clicked",
        "on_pushButtonReadAbort_clicked",
        "on_pushButtonRestore_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'progress'
        QtMocHelpers::SlotData<void(int, int, double, QString, uint64_t)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Double, 5 }, { QMetaType::QString, 6 },
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'catalog_readed'
        QtMocHelpers::SlotData<void(TapeCatalog *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'change_pos'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'error_message'
        QtMocHelpers::SlotData<void(QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'change_cache'
        QtMocHelpers::SlotData<void(uint64_t)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 16 },
        }}),
        // Slot 'log'
        QtMocHelpers::SlotData<void(int, QString)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 14 },
        }}),
        // Slot 'worker_writefilelist_AddFile'
        QtMocHelpers::SlotData<void(QString, qint64)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 }, { QMetaType::LongLong, 16 },
        }}),
        // Slot 'worker_writefilelist_CatalogReady'
        QtMocHelpers::SlotData<void(qint64)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 22 },
        }}),
        // Slot 'worker_writefilelist_Clear'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'tapeStatus'
        QtMocHelpers::SlotData<void(DWORD)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 25, 26 },
        }}),
        // Slot 'on_pushButtonWriteAddFile_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonWriteClear_clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonWriteWrite_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonWriteAddDir_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonWriteAbort_clicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonOpen_clicked'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSeek_clicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonGetPos_clicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonRead_clicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonScan_clicked'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonRestoreAll_clicked'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonSeekFirstFree_clicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonNext_clicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonExport_clicked'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonReadAbort_clicked'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButtonRestore_clicked'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<uint64_t>>(_a[5]))); break;
        case 1: _t->catalog_readed((*reinterpret_cast< std::add_pointer_t<TapeCatalog*>>(_a[1]))); break;
        case 2: _t->change_pos(); break;
        case 3: _t->error_message((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->change_cache((*reinterpret_cast< std::add_pointer_t<uint64_t>>(_a[1]))); break;
        case 5: _t->log((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->worker_writefilelist_AddFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 7: _t->worker_writefilelist_CatalogReady((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 8: _t->worker_writefilelist_Clear(); break;
        case 9: _t->tapeStatus((*reinterpret_cast< std::add_pointer_t<DWORD>>(_a[1]))); break;
        case 10: _t->on_pushButtonWriteAddFile_clicked(); break;
        case 11: _t->on_pushButtonWriteClear_clicked(); break;
        case 12: _t->on_pushButtonWriteWrite_clicked(); break;
        case 13: _t->on_pushButtonWriteAddDir_clicked(); break;
        case 14: _t->on_pushButtonWriteAbort_clicked(); break;
        case 15: _t->on_pushButtonOpen_clicked(); break;
        case 16: _t->on_pushButtonSeek_clicked(); break;
        case 17: _t->on_pushButtonGetPos_clicked(); break;
        case 18: _t->on_pushButtonRead_clicked(); break;
        case 19: _t->on_pushButtonScan_clicked(); break;
        case 20: _t->on_pushButtonRestoreAll_clicked(); break;
        case 21: _t->on_pushButtonSeekFirstFree_clicked(); break;
        case 22: _t->on_pushButtonNext_clicked(); break;
        case 23: _t->on_pushButtonExport_clicked(); break;
        case 24: _t->on_pushButtonReadAbort_clicked(); break;
        case 25: _t->on_pushButtonRestore_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 26;
    }
    return _id;
}
QT_WARNING_POP
