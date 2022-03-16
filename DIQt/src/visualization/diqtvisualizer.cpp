#include "diqtvisualizer_p.h"
#include "ui_diqtvisualizer.h"

DIQtVisualizer::DIQtVisualizer(QWidget* parent)
    : QWidget(parent)
    , d(new DIQtVisualizerPrivate(this))
{
}

DIQtVisualizer::~DIQtVisualizer()
{
    delete d;
}

void DIQtVisualizer::visualize(QObject* object)
{
    DIQtModule* module = DIQtModulePrivate::objectModule(object);
    this->visualize(module);
}

void DIQtVisualizer::visualize(DIQtModule* module)
{
    if (!module) {
        qWarning() << "module cannot be null";
        return;
    }

    d->providerService->setModule(module);
    this->show();
}

DIQtVisualizerPrivate::DIQtVisualizerPrivate(DIQtVisualizer* q)
    : QObject { q }
    , ui(new Ui::DIQtVisualizer)
{
    this->q = q;
    this->ui->setupUi(q);

    this->ui->providerView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->providerView->setModel(&this->providerModel);

    this->visualizerModule.provideAtRoot<DIQtProviderService>();
    this->visualizerModule.bootstrap(q);

    this->ui->providerView->setColumnWidth(0, 220);
}

DIQtVisualizerPrivate::~DIQtVisualizerPrivate()
{
    delete ui;
}

void DIQtVisualizerPrivate::setProviderService(DIQtProviderService* providerService)
{
    this->providerService = providerService;
}
