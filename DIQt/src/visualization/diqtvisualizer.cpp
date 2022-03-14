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

    this->show();
}

DIQtVisualizerPrivate::DIQtVisualizerPrivate(DIQtVisualizer* q)
    : QObject { q }
    , ui(new Ui::DIQtVisualizer)
{
    this->q = q;
    this->ui->setupUi(q);

    this->visualizerModule.provideAtRoot<DIQtProviderService>();
    this->visualizerModule.bootstrap(q);
}

DIQtVisualizerPrivate::~DIQtVisualizerPrivate()
{
    delete ui;
}

void DIQtVisualizerPrivate::setProviderService(DIQtProviderService* providerService)
{
    this->providerService = providerService;
}
