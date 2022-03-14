#include "diqt.h"

#include "diqtmodule_p.h"

void DIQt::inject(QObject* object)
{
    DIQtModule* module = DIQtModulePrivate::objectModule(object);
    if (!module) {
        return;
    }
    module->d->injectRecursive(object);
}

void DIQt::project(QObject* source, QObject* destination)
{
    DIQtModule* module = DIQtModulePrivate::objectModule(source);
    if (!module) {
        return;
    }
    module->bootstrap(destination);
}
