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

#include "diqt.h"
#include "diqtprovideentry.h"
#include "diqttype.h"

class DIQtInjector;
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

    template <class TScope, class T>
    void provide()
    {
        this->provideWithDefaultConstructor(DIQtType::makeType<TScope>(), DIQtType::makeType<T>());
    }

private:
    void registerType(const DIQtType& type);
    void provideWithReadyObject(const DIQtType& provider, const DIQtType& type, QObject* object);
    void provideWithDefaultConstructor(const DIQtType& provider, const DIQtType& type);

private:
    DIQtModulePrivate* d;

    friend void DIQt::inject(QObject* node);
    friend class DIQtProviderService;
};

Q_DECLARE_METATYPE(DIQtModule*)

#endif
