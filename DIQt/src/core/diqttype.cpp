/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#include <DIQt>

QMap<int, DIQtType> DIQtType::typeById;
QMap<const QMetaObject*, DIQtType> DIQtType::typeByMetaObject;

DIQtType::DIQtType()
{
    this->id = -1;
    this->metaObject = 0;
}

DIQtType::DIQtType(const QMetaObject* metaObject, int id)
{
    this->id = id;
    this->metaObject = metaObject;

    if (!DIQtType::typeByMetaObject.contains(metaObject)) {
        DIQtType::typeByMetaObject[metaObject] = *this;
    }

    if (!DIQtType::typeById.contains(id)) {
        DIQtType::typeById[id] = *this;
    }
}

const QMetaObject* DIQtType::getMetaObject() const
{
    return this->metaObject;
}

int DIQtType::getId() const
{
    return this->id;
}

bool DIQtType::operator==(const DIQtType& o) const
{
    return this->metaObject == o.metaObject;
}

DIQtType DIQtType::fromId(int id)
{
    if (DIQtType::typeById.contains(id)) {
        return DIQtType::typeById[id];
    }

    return DIQtType();
}

DIQtType DIQtType::fromObject(QObject* object)
{
    const QMetaObject* metaObject = object->metaObject();
    if (DIQtType::typeByMetaObject.contains(metaObject)) {
        return DIQtType::typeByMetaObject[metaObject];
    }

    return DIQtType();
}
