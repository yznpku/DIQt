#include "countservice.h"

CountService::CountService(QObject* parent)
    : QObject(parent)
{
}

int CountService::currentCount() const
{
    return this->count;
}

void CountService::increment()
{
    this->count += 1;
    emit this->countChanged();
}
