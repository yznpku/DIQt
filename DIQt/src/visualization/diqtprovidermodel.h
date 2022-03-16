#ifndef DIQTPROVIDERMODEL_H
#define DIQTPROVIDERMODEL_H

#include "diqtproviderservice.h"
#include <QStandardItemModel>

class DIQtProviderModel : public QStandardItemModel {
    Q_OBJECT

public:
    DIQtProviderModel(QObject* parent = nullptr);
    ~DIQtProviderModel();

public slots:
    DI_CONSUMER void setProviderService(DIQtProviderService* providerService);
    void updateData();

private:
    DIQtProviderService* providerService { nullptr };

    struct Private;
    Private* d;
};

#endif // DIQTPROVIDERMODEL_H
