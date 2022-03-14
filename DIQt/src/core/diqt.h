#ifndef DIQT_H
#define DIQT_H

#include <QObject>

namespace DIQt {
void inject(QObject* object);
void project(QObject* source, QObject* destination);
}

#ifndef Q_MOC_RUN
#define DI_MODULE
#define DI_CONSUMER
#define DI_ONINIT
#define DI_PARENT
#define DI_INJECTABLE
#endif

#endif // DIQT_H
