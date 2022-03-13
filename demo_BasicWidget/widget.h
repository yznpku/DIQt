#ifndef WIDGET_H
#define WIDGET_H

#include <DIQt>
#include <QWidget>

namespace Ui {
class Widget;
}

class CountService;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = 0);
    ~Widget();

public slots:
    DI_CONSUMER void setCountService(CountService* countService);

private slots:
    void increment();
    void onCountChanged();

private:
    Ui::Widget* ui;
    CountService* countService;
};

#endif // WIDGET_H
