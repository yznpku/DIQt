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
    if (d->module) {
        disconnect(d->module->d, nullptr, this, nullptr);
    }

    d->module = module;
    connect(d->module->d, &DIQtModulePrivate::providersChanged, this, &DIQtProviderService::entriesChanged);
    emit this->entriesChanged();
}

QList<DIQtProvideEntry> DIQtProviderService::getProvideEntries() const
{
    if (!d->module) {
        return {};
    }

    return d->module->d->providers;
}
