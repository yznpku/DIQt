#include "diqt.h"

#include "diqtmodule_p.h"

void DIQt::inject(QObject* node)
{
    QObject* root = DIQtModulePrivate::objectRoot(node);

    if (!root->dynamicPropertyNames().contains("DIQt_module")) {
        return;
    }

    DIQtModule* module = root->property("DIQt_module").value<DIQtModule*>();
    module->d->injectRecursive(node);
}

void DIQt::project(QObject* source, QObject* destination)
{
    QObject* root = DIQtModulePrivate::objectRoot(source);

    if (!root->dynamicPropertyNames().contains("DIQt_module")) {
        return;
    }

    DIQtModule* module = root->property("DIQt_module").value<DIQtModule*>();
    module->bootstrap(destination);
}
