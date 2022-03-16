#ifndef DIQTMODULE_P_H
#define DIQTMODULE_P_H

#include "diqtmodule.h"

#include "diqtinjector.h"
#include "diqtprovideentry.h"
#include "diqttype.h"

class DIQtModulePrivate : public QObject {
    Q_OBJECT

public:
    DIQtModule* q;
    QList<DIQtType> registeredTypes;
    QList<DIQtProvideEntry> providers;
    DIQtInjector* rootInjector { nullptr };

public:
    DIQtModulePrivate(DIQtModule* q);
    virtual ~DIQtModulePrivate();

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

    static QObject* objectParent(QObject* object);
    static QObject* objectRoot(QObject* object);
    static DIQtModule* objectModule(QObject* object);

signals:
    void providersChanged();
};

#endif // DIQTMODULE_P_H
