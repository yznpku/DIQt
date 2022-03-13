/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#ifndef DIQTMODULE_H
#define DIQTMODULE_H

#include <QtCore>

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
#endif

#include "diqtprovideentry.h"
#include "diqttype.h"

#include "DIQt_global.h"

class DIQtInjector;

namespace DIQt {
void inject(QObject* node);
void project(QObject* source, QObject* destination);
}

class DIQT_EXPORT DIQtModule {
public:
    DIQtModule();
    virtual ~DIQtModule();

    void bootstrap(QObject* root);

    template <class T>
    void registerType()
    {
        this->registerType(DIQtType::makeType<T>());
    }

    template <class T>
    void provideAtRoot()
    {
        this->provideWithDefaultConstructor(DIQtType(), DIQtType::makeType<T>());
    }

    template <class T>
    void provideAtRoot(T* object)
    {
        this->provideWithReadyObject(DIQtType(), DIQtType::makeType<T>(), object);
    }

    template <class TProvider, class T>
    void provide()
    {
        this->provideWithDefaultConstructor(DIQtType::makeType<TProvider>(), DIQtType::makeType<T>());
    }

protected:
    QList<DIQtType> registeredTypes;
    QList<DIQtProvideEntry> providers;

    DIQtInjector* rootInjector;

private:
    friend void DIQt::inject(QObject* node);

    void registerType(const DIQtType& type);
    void provideWithReadyObject(const DIQtType& provider, const DIQtType& type, QObject* object);
    void provideWithDefaultConstructor(const DIQtType& provider, const DIQtType& type);

    void injectAll(QObject* node);
    void injectRecursive(QObject* root);
    void injectRecursive(QGraphicsItem* root);
    void createInjector(QObject* node);
    void createRootInjector();

    void injectNode(QObject* node, DIQtInjector* injector, QList<QPair<DIQtType, QMetaMethod>>& methods, QList<QPair<DIQtType, QMetaProperty>>& properties);

    QList<QPair<DIQtType, QMetaMethod>> collectConsumerMethods(QObject* node);
    QList<QPair<DIQtType, QMetaProperty>> collectConsumerProperties(QObject* node);
    QList<QMetaMethod> collectInitMethods(QObject* node);

    bool testIncompleteMethods(QObject* node, const QList<QPair<DIQtType, QMetaMethod>>& methods);
    bool testIncompleteProperties(QObject* node, const QList<QPair<DIQtType, QMetaProperty>>& properties);
};

Q_DECLARE_METATYPE(DIQtModule*)

#endif
