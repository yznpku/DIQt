#include "diqtprovidermodel.h"

struct DIQtProviderModel::Private {
    QList<DIQtProvideEntry> cachedData;

    QHash<DIQtType, QStandardItem*> itemOfScope;
};

DIQtProviderModel::DIQtProviderModel(QObject* parent)
    : QStandardItemModel { parent }
    , d { new Private }
{
    this->setHorizontalHeaderLabels({ "Scope / Object Class", "Object Source" });
}

DIQtProviderModel::~DIQtProviderModel()
{
    delete d;
}

void DIQtProviderModel::setProviderService(DIQtProviderService* providerService)
{
    this->providerService = providerService;
    connect(providerService, &DIQtProviderService::entriesChanged, this, &DIQtProviderModel::updateData);
    this->updateData();
}

void DIQtProviderModel::updateData()
{
    d->cachedData = this->providerService->getProvideEntries();

    // clear scopes
    this->removeRows(0, this->rowCount());
    d->itemOfScope.clear();

    // add rows
    for (const auto& entry : qAsConst(d->cachedData)) {
        // find scope item
        QStandardItem* scopeItem;
        if (d->itemOfScope.contains(entry.scope)) {
            scopeItem = d->itemOfScope[entry.scope];
        } else {
            // create new scope item
            const QMetaObject* scope = entry.scope.getMetaObject();
            scopeItem = new QStandardItem { scope ? scope->className() : "Root" };
            this->appendRow(scopeItem);
        }

        const QMetaObject* objectClass = entry.type.getMetaObject();
        auto objectClassItem = new QStandardItem { objectClass ? objectClass->className() : "null" };

        QString objectSource;
        switch (entry.configuration) {
        case DIQtProvideEntry::ReadyObjectProvider:
            objectSource = "Ready Object";
            break;
        case DIQtProvideEntry::DefaultConstructorProvider:
            objectSource = "Default Constructor";
            break;
        default:;
        }
        auto objectSourceItem = new QStandardItem { objectSource };

        scopeItem->appendRow({ objectClassItem, objectSourceItem });
    }
}
