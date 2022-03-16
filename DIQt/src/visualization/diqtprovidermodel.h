#ifndef DIQTPROVIDERMODEL_H
#define DIQTPROVIDERMODEL_H

#include "diqtproviderservice.h"
#include <QAbstractTableModel>

class DIQtProviderModel : public QAbstractTableModel {
    Q_OBJECT

public:
    DIQtProviderModel(QObject* parent = nullptr);
    ~DIQtProviderModel();

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    DI_CONSUMER void setProviderService(DIQtProviderService* providerService);
    void updateData();

private:
    DIQtProviderService* providerService { nullptr };

    struct Private;
    Private* d;
};

#endif // DIQTPROVIDERMODEL_H
