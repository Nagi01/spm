/****************************************************************************
** Meta object code from reading C++ file 'qt_toolbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qt_toolbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_toolbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Qt_toolBox_t {
    QByteArrayData data[20];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qt_toolBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qt_toolBox_t qt_meta_stringdata_Qt_toolBox = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Qt_toolBox"
QT_MOC_LITERAL(1, 11, 10), // "windowInfo"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 14), // "dialogFileOpen"
QT_MOC_LITERAL(4, 38, 14), // "dialogFileSave"
QT_MOC_LITERAL(5, 53, 13), // "dialogDirPath"
QT_MOC_LITERAL(6, 67, 13), // "dialogMsgWarn"
QT_MOC_LITERAL(7, 81, 11), // "dialogMsgOK"
QT_MOC_LITERAL(8, 93, 14), // "dialogMsgYesNo"
QT_MOC_LITERAL(9, 108, 11), // "closeDialog"
QT_MOC_LITERAL(10, 120, 15), // "diaplayFileInfo"
QT_MOC_LITERAL(11, 136, 11), // "QModelIndex"
QT_MOC_LITERAL(12, 148, 5), // "index"
QT_MOC_LITERAL(13, 154, 18), // "clickButtonToggled"
QT_MOC_LITERAL(14, 173, 18), // "clickPropertyClear"
QT_MOC_LITERAL(15, 192, 14), // "clickDispImage"
QT_MOC_LITERAL(16, 207, 22), // "clickDispPropertyImage"
QT_MOC_LITERAL(17, 230, 16), // "activeScaleCombo"
QT_MOC_LITERAL(18, 247, 22), // "clickSurgicalDetection"
QT_MOC_LITERAL(19, 270, 30) // "clickPropertySurgicalDetection"

    },
    "Qt_toolBox\0windowInfo\0\0dialogFileOpen\0"
    "dialogFileSave\0dialogDirPath\0dialogMsgWarn\0"
    "dialogMsgOK\0dialogMsgYesNo\0closeDialog\0"
    "diaplayFileInfo\0QModelIndex\0index\0"
    "clickButtonToggled\0clickPropertyClear\0"
    "clickDispImage\0clickDispPropertyImage\0"
    "activeScaleCombo\0clickSurgicalDetection\0"
    "clickPropertySurgicalDetection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qt_toolBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    1,  102,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    1,  109,    2, 0x08 /* Private */,
      18,    0,  112,    2, 0x08 /* Private */,
      19,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Qt_toolBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Qt_toolBox *_t = static_cast<Qt_toolBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->windowInfo(); break;
        case 1: _t->dialogFileOpen(); break;
        case 2: _t->dialogFileSave(); break;
        case 3: _t->dialogDirPath(); break;
        case 4: _t->dialogMsgWarn(); break;
        case 5: _t->dialogMsgOK(); break;
        case 6: _t->dialogMsgYesNo(); break;
        case 7: _t->closeDialog(); break;
        case 8: _t->diaplayFileInfo((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->clickButtonToggled(); break;
        case 10: _t->clickPropertyClear(); break;
        case 11: _t->clickDispImage(); break;
        case 12: _t->clickDispPropertyImage(); break;
        case 13: _t->activeScaleCombo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->clickSurgicalDetection(); break;
        case 15: _t->clickPropertySurgicalDetection(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Qt_toolBox::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Qt_toolBox.data,
      qt_meta_data_Qt_toolBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Qt_toolBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qt_toolBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Qt_toolBox.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Qt_toolBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
