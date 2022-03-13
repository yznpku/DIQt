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

class DIQtInjector;

namespace DIQt {
void inject(QObject* node);
void project(QObject* source, QObject* destination);
}

class DIQtModulePrivate;
class DIQtModule {
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

private:
    void registerType(const DIQtType& type);
    void provideWithReadyObject(const DIQtType& provider, const DIQtType& type, QObject* object);
    void provideWithDefaultConstructor(const DIQtType& provider, const DIQtType& type);

private:
    DIQtModulePrivate* d;
    friend void DIQt::inject(QObject* node);
};

Q_DECLARE_METATYPE(DIQtModule*)

#endif
