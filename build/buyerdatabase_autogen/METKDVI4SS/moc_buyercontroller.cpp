/****************************************************************************
** Meta object code from reading C++ file 'buyercontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/controllers/buyercontroller.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buyercontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN15BuyerControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto BuyerController::qt_create_metaobjectdata<qt_meta_tag_ZN15BuyerControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BuyerController",
        "fileOpened",
        "",
        "fileName",
        "fileSaved",
        "error",
        "message",
        "buyerAdded",
        "row",
        "buyerEdited",
        "buyerDeleted",
        "searchResultsUpdated",
        "QList<Buyer>",
        "results",
        "sortOrderChanged",
        "column",
        "Qt::SortOrder",
        "order"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'fileOpened'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'fileSaved'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Signal 'buyerAdded'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Signal 'buyerEdited'
        QtMocHelpers::SignalData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Signal 'buyerDeleted'
        QtMocHelpers::SignalData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Signal 'searchResultsUpdated'
        QtMocHelpers::SignalData<void(const QVector<Buyer> &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Signal 'sortOrderChanged'
        QtMocHelpers::SignalData<void(int, Qt::SortOrder)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { 0x80000000 | 16, 17 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BuyerController, qt_meta_tag_ZN15BuyerControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BuyerController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15BuyerControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15BuyerControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15BuyerControllerE_t>.metaTypes,
    nullptr
} };

void BuyerController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BuyerController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->fileOpened((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->fileSaved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->buyerAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->buyerEdited((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->buyerDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->searchResultsUpdated((*reinterpret_cast< std::add_pointer_t<QList<Buyer>>>(_a[1]))); break;
        case 7: _t->sortOrderChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Qt::SortOrder>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(const QString & )>(_a, &BuyerController::fileOpened, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(const QString & )>(_a, &BuyerController::fileSaved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(const QString & )>(_a, &BuyerController::error, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(int )>(_a, &BuyerController::buyerAdded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(int )>(_a, &BuyerController::buyerEdited, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(int )>(_a, &BuyerController::buyerDeleted, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(const QVector<Buyer> & )>(_a, &BuyerController::searchResultsUpdated, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerController::*)(int , Qt::SortOrder )>(_a, &BuyerController::sortOrderChanged, 7))
            return;
    }
}

const QMetaObject *BuyerController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BuyerController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15BuyerControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BuyerController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void BuyerController::fileOpened(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void BuyerController::fileSaved(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void BuyerController::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void BuyerController::buyerAdded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void BuyerController::buyerEdited(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void BuyerController::buyerDeleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void BuyerController::searchResultsUpdated(const QVector<Buyer> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void BuyerController::sortOrderChanged(int _t1, Qt::SortOrder _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}
QT_WARNING_POP
