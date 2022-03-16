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
        switch (index.column()) {
        case 0:
            return "Scope";
        case 1:
            return "Object Class";
        case 2:
            return "Object Source";
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
}
