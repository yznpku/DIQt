#ifndef DIQT_H
#define DIQT_H

#include <QObject>

namespace DIQt {
void inject(QObject* node);
void project(QObject* source, QObject* destination);
}

#endif // DIQT_H
