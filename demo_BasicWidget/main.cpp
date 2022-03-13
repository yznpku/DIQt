#include <QApplication>

#include "countservice.h"
#include "widget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    DIQtModule module;
    module.provideAtRoot<CountService>();

    Widget widget;
    module.bootstrap(&widget);

    widget.show();
    return app.exec();
}
