#include "widget.h"
#include "ui_widget.h"

#include "countservice.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , countService(0)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setCountService(CountService* countService)
{
    this->countService = countService;

    connect(ui->button, SIGNAL(clicked(bool)), this, SLOT(increment()));
    connect(this->countService, SIGNAL(countChanged()), this, SLOT(onCountChanged()));
}

void Widget::increment()
{
    this->countService->increment();
}

void Widget::onCountChanged()
{
    ui->label->setText(QString::number(this->countService->currentCount()));
}
