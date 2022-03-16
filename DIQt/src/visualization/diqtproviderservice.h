#ifndef DIQTPROVIDERSERVICE_H
#define DIQTPROVIDERSERVICE_H

#include "core/diqtmodule.h"
#include "core/diqttype.h"
#include <QObject>

class DIQtProviderServicePrivate;
class DIQtProviderService : public QObject {
    Q_OBJECT

    DIQtProviderServicePrivate* d;

signals:
    void entriesChanged();

public:
    Q_INVOKABLE DIQtProviderService(QObject* parent = nullptr);
    virtual ~DIQtProviderService();

    void setModule(DIQtModule* module);
    QList<DIQtProvideEntry> getProvideEntries() const;
};

#endif // DIQTPROVIDERSERVICE_H
