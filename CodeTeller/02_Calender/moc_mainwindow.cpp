/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    QByteArrayData data[22];
    char stringdata0[359];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Window"
QT_MOC_LITERAL(1, 7, 13), // "localeChanged"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 5), // "index"
QT_MOC_LITERAL(4, 28, 15), // "firstDayChanged"
QT_MOC_LITERAL(5, 44, 20), // "selectionModeChanged"
QT_MOC_LITERAL(6, 65, 23), // "horizontalHeaderChanged"
QT_MOC_LITERAL(7, 89, 21), // "verticalHeaderChanged"
QT_MOC_LITERAL(8, 111, 19), // "selectedDateChanged"
QT_MOC_LITERAL(9, 131, 18), // "minimumDateChanged"
QT_MOC_LITERAL(10, 150, 4), // "date"
QT_MOC_LITERAL(11, 155, 18), // "maximumDateChanged"
QT_MOC_LITERAL(12, 174, 20), // "weekdayFormatChanged"
QT_MOC_LITERAL(13, 195, 20), // "weekendFormatChanged"
QT_MOC_LITERAL(14, 216, 15), // "reformatHeaders"
QT_MOC_LITERAL(15, 232, 20), // "reformatCalendarPage"
QT_MOC_LITERAL(16, 253, 16), // "handleCodeViewer"
QT_MOC_LITERAL(17, 270, 14), // "handleAnalysis"
QT_MOC_LITERAL(18, 285, 17), // "handleGraphExpand"
QT_MOC_LITERAL(19, 303, 17), // "threadTypeChanged"
QT_MOC_LITERAL(20, 321, 17), // "targetTypeChanged"
QT_MOC_LITERAL(21, 339, 19) // "AnalysisTypeChanged"

    },
    "Window\0localeChanged\0\0index\0firstDayChanged\0"
    "selectionModeChanged\0horizontalHeaderChanged\0"
    "verticalHeaderChanged\0selectedDateChanged\0"
    "minimumDateChanged\0date\0maximumDateChanged\0"
    "weekdayFormatChanged\0weekendFormatChanged\0"
    "reformatHeaders\0reformatCalendarPage\0"
    "handleCodeViewer\0handleAnalysis\0"
    "handleGraphExpand\0threadTypeChanged\0"
    "targetTypeChanged\0AnalysisTypeChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x08 /* Private */,
       4,    1,  107,    2, 0x08 /* Private */,
       5,    1,  110,    2, 0x08 /* Private */,
       6,    1,  113,    2, 0x08 /* Private */,
       7,    1,  116,    2, 0x08 /* Private */,
       8,    0,  119,    2, 0x08 /* Private */,
       9,    1,  120,    2, 0x08 /* Private */,
      11,    1,  123,    2, 0x08 /* Private */,
      12,    0,  126,    2, 0x08 /* Private */,
      13,    0,  127,    2, 0x08 /* Private */,
      14,    0,  128,    2, 0x08 /* Private */,
      15,    0,  129,    2, 0x08 /* Private */,
      16,    0,  130,    2, 0x08 /* Private */,
      17,    0,  131,    2, 0x08 /* Private */,
      18,    0,  132,    2, 0x08 /* Private */,
      19,    1,  133,    2, 0x08 /* Private */,
      20,    1,  136,    2, 0x08 /* Private */,
      21,    1,  139,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   10,
    QMetaType::Void, QMetaType::QDate,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Window *_t = static_cast<Window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->localeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->firstDayChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->selectionModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->horizontalHeaderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->verticalHeaderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->selectedDateChanged(); break;
        case 6: _t->minimumDateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 7: _t->maximumDateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 8: _t->weekdayFormatChanged(); break;
        case 9: _t->weekendFormatChanged(); break;
        case 10: _t->reformatHeaders(); break;
        case 11: _t->reformatCalendarPage(); break;
        case 12: _t->handleCodeViewer(); break;
        case 13: _t->handleAnalysis(); break;
        case 14: _t->handleGraphExpand(); break;
        case 15: _t->threadTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->targetTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->AnalysisTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Window::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Window.data,
      qt_meta_data_Window,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
