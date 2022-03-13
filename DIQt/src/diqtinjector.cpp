/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#include "diqtinjector.h"
#include <DIQt>

static bool metaObjectInherits(const QMetaObject* o1, const QMetaObject* o2)
{
    do {
        if (o1 == o2)
            return true;
    } while ((o1 = o1->superClass()));
    return false;
}

DIQtInjector::DIQtInjector(QObject* parent)
    : QObject(parent)
{
}

DIQtInjector::~DIQtInjector()
{
    for (QList<DIQtProvideEntry>::const_iterator i = this->providers.constBegin(); i != this->providers.constEnd(); i++) {
        switch (i->configuration) {
        case DIQtProvideEntry::ReadyObjectProvider:
            break;

        case DIQtProvideEntry::DefaultConstructorProvider:
            QObject* object = this->injectables[i->type];
            if (object) {
                delete object;
            }
            break;
        }
    }
}

void DIQtInjector::initProviders(const QList<DIQtProvideEntry>& providers)
{
    this->providers = providers;

    for (QList<DIQtProvideEntry>::const_iterator i = providers.constBegin(); i != providers.constEnd(); i++) {
        this->injectableTypes.prepend(i->type);
    }
}

bool DIQtInjector::canProvide(const DIQtType& type) const
{
    for (QList<DIQtType>::const_iterator i = this->injectableTypes.constBegin(); i != this->injectableTypes.constEnd(); i++) {
        if (metaObjectInherits(i->getMetaObject(), type.getMetaObject())) {
            return true;
        }
    }
    return false;
}

QObject* DIQtInjector::get(const DIQtType& type)
{
    for (QList<DIQtType>::const_iterator i = this->injectableTypes.constBegin(); i != this->injectableTypes.constEnd(); i++) {
        if (metaObjectInherits(i->getMetaObject(), type.getMetaObject())) {
            return this->getExact(*i);
        }
    }
    return 0;
}

bool DIQtInjector::injectMethod(QObject* object, const DIQtType& type, const QMetaMethod& consumerMethod)
{
    if (this->canProvide(type)) {
        consumerMethod.invoke(object, Q_ARG(QObject*, this->get(type)));
        return true;
    }
    return false;
}

bool DIQtInjector::injectProperty(QObject* object, const DIQtType& type, const QMetaProperty& consumerProperty)
{
    if (this->canProvide(type)) {
        consumerProperty.write(object, QVariant::fromValue<QObject*>(this->get(type)));
        return true;
    }
    return false;
}

QObject* DIQtInjector::getExact(const DIQtType& type)
{
    if (this->injectables.contains(type)) {
        return this->injectables[type];
    }

    for (QList<DIQtProvideEntry>::const_iterator i = this->providers.constBegin(); i != this->providers.constEnd(); i++) {
        if (i->type == type) {
            switch (i->configuration) {
            case DIQtProvideEntry::ReadyObjectProvider:
                this->injectables[i->type] = i->readyObject;
                break;

            case DIQtProvideEntry::DefaultConstructorProvider:
                QObject* newObject = type.getMetaObject()->newInstance();
                newObject->setParent(this);
                DIQt::inject(newObject);

                this->injectables[i->type] = reinterpret_cast<QObject*>(newObject);
                break;
            }

            return this->injectables[i->type];
        }
    }

    return 0;
}
