﻿/*
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

static QObject* objectParent(QObject* object)
{
    if (object->dynamicPropertyNames().contains("DIQt_parent")) {
        QObject* parent = object->property("DIQt_parent").value<QObject*>();
        return parent;

    } else if (QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(object)) {
        QGraphicsItem* parent = item->parentItem();
        while (parent && !dynamic_cast<QObject*>(parent)) {
            parent = parent->parentItem();
        }

        if (!parent) {
            return item->scene();
        }

        return dynamic_cast<QObject*>(parent);

    } else if (QWidget* widget = dynamic_cast<QWidget*>(object)) {
        return widget->parentWidget();

    } else {
        return object->parent();
    }
}

static QObject* objectRoot(QObject* object)
{
    QObject* parent = objectParent(object);
    while (parent) {
        object = parent;
        parent = objectParent(object);
    }
    return object;
}

void DIQt::inject(QObject* node)
{
    QObject* root = objectRoot(node);

    if (!root->dynamicPropertyNames().contains("DIQt_module")) {
        return;
    }

    DIQtModule* module = root->property("DIQt_module").value<DIQtModule*>();
    module->injectRecursive(node);
}

void DIQt::project(QObject* source, QObject* destination)
{
    QObject* root = objectRoot(source);

    if (!root->dynamicPropertyNames().contains("DIQt_module")) {
        return;
    }

    DIQtModule* module = root->property("DIQt_module").value<DIQtModule*>();
    module->bootstrap(destination);
}

DIQtModule::DIQtModule()
{
    this->rootInjector = 0;
#ifdef QT_QUICK_LIB
    qmlRegisterType<DIQtInjector>("DIQt", 1, 0, "DIQtInjector");
#endif
}

DIQtModule::~DIQtModule()
{
    if (this->rootInjector) {
        delete this->rootInjector;
    }
}

void DIQtModule::bootstrap(QObject* root)
{
    root->setProperty("DIQt_module", QVariant::fromValue<DIQtModule*>(this));

    if (!this->rootInjector) {
        this->createRootInjector();
    }
    this->injectRecursive(root);
}

void DIQtModule::registerType(const DIQtType& type)
{
    DIQtType::typeById[type.getId()] = type;
    this->registeredTypes.append(type);
}

void DIQtModule::provideWithReadyObject(const DIQtType& provider, const DIQtType& type, QObject* object)
{
    DIQtProvideEntry entry;
    entry.provider = provider;
    entry.type = type;
    entry.configuration = DIQtProvideEntry::ReadyObjectProvider;
    entry.readyObject = object;
    this->providers.append(entry);
}

void DIQtModule::provideWithDefaultConstructor(const DIQtType& provider, const DIQtType& type)
{
    DIQtProvideEntry entry;
    entry.provider = provider;
    entry.type = type;
    entry.configuration = DIQtProvideEntry::DefaultConstructorProvider;
    entry.readyObject = 0;

    const QMetaObject* metaObject = type.getMetaObject();
    for (int i = 0; i < metaObject->constructorCount(); i++) {
        const QMetaMethod& method = metaObject->constructor(i);
        if (method.parameterTypes().size() == 0) {
            entry.defaultConstructor = method;
            break;
        }
    }

#if QT_VERSION_MAJOR > 4
    if (!entry.defaultConstructor.isValid()) {
        qWarning() << type.getMetaObject()->className() << "cannot be provided since it has no Q_INVOKABLE constructors.";
        return;
    }
#endif

    this->providers.append(entry);
}

void DIQtModule::injectAll(QObject* node)
{
    if (node->dynamicPropertyNames().contains("DIQt_injected") && node->property("DIQt_injected").toBool()) {
        return;
    }

    this->createInjector(node);

    QList<QPair<DIQtType, QMetaMethod>> consumerMethods = this->collectConsumerMethods(node);
    QList<QPair<DIQtType, QMetaProperty>> consumerProperties = this->collectConsumerProperties(node);
    QList<QMetaMethod> initMethods = this->collectInitMethods(node);

    // inject with class injectors
    QObject* head = node;
    while (consumerMethods.size() && head) {
        if (head->dynamicPropertyNames().contains("DIQt_injector")) {
            DIQtInjector* injector = head->property("DIQt_injector").value<DIQtInjector*>();
            this->injectNode(node, injector, consumerMethods, consumerProperties);
        }

        // set head to its parent
        head = objectParent(head);
    }

    // inject with root injector
    this->injectNode(node, this->rootInjector, consumerMethods, consumerProperties);

    node->setProperty("DIQt_injected", true);

    // test if all methods are injected
    if (testIncompleteMethods(node, consumerMethods) && testIncompleteProperties(node, consumerProperties)) {
        // call onInit method
        for (int i = 0; i < initMethods.size(); i++) {
            const QMetaMethod& method = initMethods[i];
            method.invoke(node);
        }
    }
}

void DIQtModule::injectRecursive(QObject* root)
{
    // inject this node
    this->injectAll(root);

    // inject QObject children
    QList<QObject*> objectChildren = root->children();
    for (QList<QObject*>::const_iterator i = objectChildren.constBegin(); i != objectChildren.constEnd(); i++) {
        QObject* child = *i;
        this->injectRecursive(child);
    }

    // inject QGraphicsScene items
    if (QGraphicsScene* scene = dynamic_cast<QGraphicsScene*>(root)) {
        const QList<QGraphicsItem*>& items = scene->items();
        QSet<QGraphicsItem*> topItems;
        for (QList<QGraphicsItem*>::const_iterator i = items.constBegin(); i != items.constEnd(); i++) {
            QGraphicsItem* item = *i;
            topItems.insert(item->topLevelItem());
        }
        for (QSet<QGraphicsItem*>::const_iterator i = topItems.constBegin(); i != topItems.constEnd(); i++) {
            this->injectRecursive(*i);
        }
    }
}

void DIQtModule::injectRecursive(QGraphicsItem* root)
{
    // inject this node if it is QObject
    if (QGraphicsObject* object = root->toGraphicsObject()) {
        this->injectAll(object);

        // inject QObject children
        QList<QObject*> objectChildren = object->children();
        for (QList<QObject*>::const_iterator i = objectChildren.constBegin(); i != objectChildren.constEnd(); i++) {
            QObject* child = *i;
            this->injectRecursive(child);
        }
    }

    // inject QGraphicsItem children
    const QList<QGraphicsItem*>& itemChildren = root->childItems();
    for (QList<QGraphicsItem*>::const_iterator i = itemChildren.constBegin(); i != itemChildren.constEnd(); i++) {
        QGraphicsItem* child = *i;
        this->injectRecursive(child);
    }
}

void DIQtModule::createInjector(QObject* node)
{
    QList<DIQtProvideEntry> nodeProviders;
    for (QList<DIQtProvideEntry>::const_iterator i = this->providers.constBegin(); i != this->providers.constEnd(); i++) {
        const DIQtType& type = i->provider;
        if (type.getMetaObject() && metaObjectInherits(type.getMetaObject(), node->metaObject())) {
            nodeProviders.append(*i);
        }
    }

    if (!nodeProviders.isEmpty()) {
        DIQtInjector* injector = new DIQtInjector(node);
        injector->initProviders(nodeProviders);
        node->setProperty("DIQt_injector", QVariant::fromValue<DIQtInjector*>(injector));
    }
}

void DIQtModule::createRootInjector()
{
    this->rootInjector = new DIQtInjector();
    this->rootInjector->setProperty("DIQt_module", QVariant::fromValue<DIQtModule*>(this));

    QList<DIQtProvideEntry> rootProviders;
    for (QList<DIQtProvideEntry>::const_iterator i = this->providers.constBegin(); i != this->providers.constEnd(); i++) {
        const DIQtType& type = i->provider;
        if (!type.getMetaObject()) {
            rootProviders.append(*i);
        }
    }

    this->rootInjector->initProviders(rootProviders);
}

void DIQtModule::injectNode(QObject* node, DIQtInjector* injector, QList<QPair<DIQtType, QMetaMethod>>& methods, QList<QPair<DIQtType, QMetaProperty>>& properties)
{
    for (QList<QPair<DIQtType, QMetaMethod>>::iterator i = methods.begin(); i != methods.end();) {
        const DIQtType& type = i->first;
        const QMetaMethod& method = i->second;
        if (injector->injectMethod(node, type, method)) {
            i = methods.erase(i);
        } else {
            i++;
        }
    }

    for (QList<QPair<DIQtType, QMetaProperty>>::iterator i = properties.begin(); i != properties.end();) {
        const DIQtType& type = i->first;
        const QMetaProperty& property = i->second;
        if (injector->injectProperty(node, type, property)) {
            i = properties.erase(i);
        } else {
            i++;
        }
    }
}

QList<QPair<DIQtType, QMetaMethod>> DIQtModule::collectConsumerMethods(QObject* node)
{
    QList<QPair<DIQtType, QMetaMethod>> consumerMethods;
    const QMetaObject* metaObject = node->metaObject();

    for (int i = 0; i < metaObject->methodCount(); i++) {
        const QMetaMethod& method = metaObject->method(i);
        if (QString(method.tag()) == "DI_CONSUMER" && method.parameterTypes().size() == 1) {
            int typeId = QMetaType::type(method.parameterTypes()[0].constData());
#if QT_VERSION_MAJOR > 4
            if (typeId == QMetaType::UnknownType) {
                qWarning() << "Cannot inject method" << method.name() << "in" << node << ":" << method.parameterTypes()[0] << "is not registered";
            }
#endif
            const DIQtType& type = DIQtType::fromId(typeId);
            consumerMethods.append(QPair<DIQtType, QMetaMethod>(type, method));
        }
    }

    return consumerMethods;
}

QList<QPair<DIQtType, QMetaProperty>> DIQtModule::collectConsumerProperties(QObject* node)
{
    QList<QPair<DIQtType, QMetaProperty>> consumerProperties;
    const QMetaObject* metaObject = node->metaObject();

    for (int i = 0; i < metaObject->propertyCount(); i++) {
        const QMetaProperty& property = metaObject->property(i);
        if (QString(property.name()).startsWith("di_consumer_")) {
            int typeId = QMetaType::type(property.typeName());
#if QT_VERSION_MAJOR > 4
            if (typeId == QMetaType::UnknownType) {
                qWarning() << "Cannot inject property" << property.name() << "in" << node << ":" << property.typeName() << "is not registered";
            }
#endif
            const DIQtType& type = DIQtType::fromId(typeId);
            consumerProperties.append(QPair<DIQtType, QMetaProperty>(type, property));
        }
    }

    return consumerProperties;
}

QList<QMetaMethod> DIQtModule::collectInitMethods(QObject* node)
{
    QList<QMetaMethod> initMethods;
    const QMetaObject* metaObject = node->metaObject();
    for (int i = 0; i < metaObject->methodCount(); i++) {
        const QMetaMethod& method = metaObject->method(i);
        if (method.parameterTypes().size() == 0) {
#if QT_VERSION_MAJOR < 5
            QString methodName = method.signature();
#else
            QString methodName = method.name();
#endif
            if (QString(method.tag()) == "DI_ONINIT"
                || QString(methodName) == "di_onInit"
                || QString(methodName).startsWith("di_onInit_")) {
                initMethods.append(method);
            }
        }
    }
    return initMethods;
}

bool DIQtModule::testIncompleteMethods(QObject* node, const QList<QPair<DIQtType, QMetaMethod>>& methods)
{
    if (methods.empty()) {
        return true;
    }

    QList<QString> incompleteMethods;
    for (QList<QPair<DIQtType, QMetaMethod>>::const_iterator i = methods.constBegin(); i != methods.constEnd(); i++) {
#if QT_VERSION_MAJOR < 5
        QString methodName = i->second.signature();
#else
        QString methodName = i->second.name();
#endif
        incompleteMethods.append(methodName);
    }

    qWarning() << "Injection incomplete for" << node << ":" << incompleteMethods;
    return false;
}

bool DIQtModule::testIncompleteProperties(QObject* node, const QList<QPair<DIQtType, QMetaProperty>>& properties)
{
    if (properties.empty()) {
        return true;
    }

    QList<QString> incompleteProperties;
    for (QList<QPair<DIQtType, QMetaProperty>>::const_iterator i = properties.constBegin(); i != properties.constEnd(); i++) {
        incompleteProperties.append(i->second.name());
    }

    qWarning() << "Injection incomplete for" << node << ":" << incompleteProperties;
    return false;
}
