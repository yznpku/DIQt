/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#ifndef DIQTPROVIDEENTRY_H
#define DIQTPROVIDEENTRY_H

#include "diqttype.h"

class DIQtProvideEntry {
public:
    enum ProviderConfiguration {
        ReadyObjectProvider,
        DefaultConstructorProvider
    };

    DIQtType scope;
    DIQtType type;
    ProviderConfiguration configuration;

    QObject* readyObject;
    QMetaMethod defaultConstructor;
};

#endif // DIQTPROVIDEENTRY_H
