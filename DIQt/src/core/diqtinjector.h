/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#ifndef DIQTINJECTOR_H
#define DIQTINJECTOR_H

#include "diqtprovideentry.h"
#include "diqttype.h"

class DIQtInjector : public QObject {
    Q_OBJECT

    QList<DIQtType> injectableTypes;
    QHash<DIQtType, QObject*> injectables;
    QList<DIQtProvideEntry> providers;

public:
    DIQtInjector(QObject* parent = 0);
    virtual ~DIQtInjector();

    void initProviders(const QList<DIQtProvideEntry>& providers);

    bool canProvide(const DIQtType& type) const;
    QObject* get(const DIQtType& type);

    bool injectMethod(QObject* object, const DIQtType& type, const QMetaMethod& consumerMethod);
    bool injectProperty(QObject* object, const DIQtType& type, const QMetaProperty& consumerProperty);

public:
    QObject* getExact(const DIQtType& type);
};

Q_DECLARE_METATYPE(DIQtInjector*)

#endif // DIQTINJECTOR_H
