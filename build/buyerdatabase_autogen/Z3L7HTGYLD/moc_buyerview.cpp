/****************************************************************************
** Meta object code from reading C++ file 'buyerview.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/views/buyerview.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buyerview.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9BuyerViewE_t {};
} // unnamed namespace

template <> constexpr inline auto BuyerView::qt_create_metaobjectdata<qt_meta_tag_ZN9BuyerViewE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BuyerView",
        "addBuyerRequested",
        "",
        "editBuyerRequested",
        "row",
        "deleteBuyerRequested",
        "searchRequested",
        "column",
        "sortRequested",
        "Qt::SortOrder",
        "order",
        "printRequested",
        "showChartRequested",
        "saveRequested",
        "openRequested",
        "fileName",
        "rowsMoved",
        "QList<int>",
        "sourceRows",
        "targetRow",
        "onHeaderClicked",
        "onCustomContextMenu",
        "point",
        "onAddBuyer",
        "onEditBuyer",
        "onDeleteBuyer",
        "onSearch",
        "onPrint",
        "onShowChart",
        "onSave",
        "onOpen"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'addBuyerRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'editBuyerRequested'
        QtMocHelpers::SignalData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'deleteBuyerRequested'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Signal 'searchRequested'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Signal 'sortRequested'
        QtMocHelpers::SignalData<void(int, Qt::SortOrder)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { 0x80000000 | 9, 10 },
        }}),
        // Signal 'printRequested'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'showChartRequested'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'saveRequested'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'openRequested'
        QtMocHelpers::SignalData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Signal 'rowsMoved'
        QtMocHelpers::SignalData<void(const QVector<int> &, int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 }, { QMetaType::Int, 19 },
        }}),
        // Slot 'onHeaderClicked'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Slot 'onCustomContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 22 },
        }}),
        // Slot 'onAddBuyer'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onEditBuyer'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteBuyer'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSearch'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPrint'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowChart'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSave'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOpen'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BuyerView, qt_meta_tag_ZN9BuyerViewE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BuyerView::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableView::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BuyerViewE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BuyerViewE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9BuyerViewE_t>.metaTypes,
    nullptr
} };

void BuyerView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BuyerView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->addBuyerRequested(); break;
        case 1: _t->editBuyerRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->deleteBuyerRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->searchRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->sortRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Qt::SortOrder>>(_a[2]))); break;
        case 5: _t->printRequested(); break;
        case 6: _t->showChartRequested(); break;
        case 7: _t->saveRequested(); break;
        case 8: _t->openRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->rowsMoved((*reinterpret_cast< std::add_pointer_t<QList<int>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->onHeaderClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->onCustomContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 12: _t->onAddBuyer(); break;
        case 13: _t->onEditBuyer(); break;
        case 14: _t->onDeleteBuyer(); break;
        case 15: _t->onSearch(); break;
        case 16: _t->onPrint(); break;
        case 17: _t->onShowChart(); break;
        case 18: _t->onSave(); break;
        case 19: _t->onOpen(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<int> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)()>(_a, &BuyerView::addBuyerRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(int )>(_a, &BuyerView::editBuyerRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(int )>(_a, &BuyerView::deleteBuyerRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(int )>(_a, &BuyerView::searchRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(int , Qt::SortOrder )>(_a, &BuyerView::sortRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)()>(_a, &BuyerView::printRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)()>(_a, &BuyerView::showChartRequested, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)()>(_a, &BuyerView::saveRequested, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(const QString & )>(_a, &BuyerView::openRequested, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (BuyerView::*)(const QVector<int> & , int )>(_a, &BuyerView::rowsMoved, 9))
            return;
    }
}

const QMetaObject *BuyerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BuyerView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9BuyerViewE_t>.strings))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int BuyerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void BuyerView::addBuyerRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BuyerView::editBuyerRequested(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void BuyerView::deleteBuyerRequested(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void BuyerView::searchRequested(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void BuyerView::sortRequested(int _t1, Qt::SortOrder _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void BuyerView::printRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BuyerView::showChartRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void BuyerView::saveRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void BuyerView::openRequested(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void BuyerView::rowsMoved(const QVector<int> & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}
QT_WARNING_POP
