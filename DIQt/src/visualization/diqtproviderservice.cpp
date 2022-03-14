#include "diqtproviderservice_p.h"

#include "core/diqtmodule_p.h"

DIQtProviderService::DIQtProviderService(QObject* parent)
    : QObject { parent }
    , d { new DIQtProviderServicePrivate }
{
}

DIQtProviderService::~DIQtProviderService()
{
    delete d;
}

void DIQtProviderService::setModule(DIQtModule* module)
{
    d->module = module;
}

QList<DIQtProvideEntry> DIQtProviderService::getProvideEntries() const
{
    if (!d->module) {
        return {};
    }

    return d->module->d->providers;
}
