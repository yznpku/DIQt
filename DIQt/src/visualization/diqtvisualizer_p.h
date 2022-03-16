#ifndef DIQTVISUALIZER_P_H
#define DIQTVISUALIZER_P_H

#include "diqtvisualizer.h"

#include "core/diqtmodule_p.h"
#include "diqtprovidermodel.h"
#include "diqtproviderservice.h"

namespace Ui {
class DIQtVisualizer;
}

class DIQtVisualizerPrivate : public QObject {
    Q_OBJECT

public:
    DIQtVisualizer* q;
    Ui::DIQtVisualizer* ui;
    DIQtModule visualizerModule;
    DIQtProviderService* providerService { nullptr };

    DIQtProviderModel providerModel { this };

public:
    DIQtVisualizerPrivate(DIQtVisualizer* q);
    ~DIQtVisualizerPrivate();

public slots:
    DI_CONSUMER void setProviderService(DIQtProviderService* providerService);
};

#endif // DIQTVISUALIZER_P_H
