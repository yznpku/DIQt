/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#ifndef DIQTTYPE_H
#define DIQTTYPE_H

#include <QtCore>

#if QT_QUICK_LIB
#include <QtQuick>
#endif

#include "DIQt_global.h"

class DIQT_EXPORT DIQtType {
    int id;
    const QMetaObject* metaObject;

public:
    DIQtType();
    DIQtType(const QMetaObject* metaObject, int id);

    const QMetaObject* getMetaObject() const;
    int getId() const;

    bool operator==(const DIQtType& o) const;

    static DIQtType fromId(int id);
    static DIQtType fromObject(QObject* object);

private:
    friend class DIQtModule;

    template <class T>
    static DIQtType makeType()
    {
        int id = qRegisterMetaType<T*>();
#if QT_QUICK_LIB
        qmlRegisterType<T>("DIQt", 1, 0, T::staticMetaObject.className());
#endif
        return DIQtType(&T::staticMetaObject, id);
    }

    static QMap<int, DIQtType> typeById;
    static QMap<const QMetaObject*, DIQtType> typeByMetaObject;
};

#if QT_VERSION_MAJOR < 5
inline uint qHash(const DIQtType& key)
{
    return qHash(key.getId());
}

#else
inline uint qHash(const DIQtType& key, uint seed)
{
    return qHash(key.getId(), seed);
}
#endif

#endif
