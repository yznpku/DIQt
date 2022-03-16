#include "diqtprovidermodel.h"

struct DIQtProviderModel::Private {
    QList<DIQtProvideEntry> cachedData;
};

DIQtProviderModel::DIQtProviderModel(QObject* parent)
    : QAbstractTableModel { parent }
    , d { new Private }
{
}

DIQtProviderModel::~DIQtProviderModel()
{
    delete d;
}

int DIQtProviderModel::rowCount(const QModelIndex& parent) const
{
    return d->cachedData.size();
}

int DIQtProviderModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

QVariant DIQtProviderModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        const DIQtProvideEntry& entry = d->cachedData[index.row()];
        switch (index.column()) {
        case 0: {
            const QMetaObject* scope = entry.scope.getMetaObject();
            return scope ? scope->className() : "Root";
        }
        case 1: {
            const QMetaObject* objectClass = entry.type.getMetaObject();
            return objectClass ? objectClass->className() : "null";
        }
        case 2:
            return entry.configuration;
        default:;
        }
    }
    return {};
}

QVariant DIQtProviderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Scope";
        case 1:
            return "Object Class";
        case 2:
            return "Object Source";
        default:;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void DIQtProviderModel::setProviderService(DIQtProviderService* providerService)
{
    this->providerService = providerService;
    connect(providerService, &DIQtProviderService::entriesChanged, this, &DIQtProviderModel::updateData);
    this->updateData();
}

void DIQtProviderModel::updateData()
{
    beginResetModel();
    d->cachedData = this->providerService->getProvideEntries();
    endResetModel();
}
