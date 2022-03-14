#include "countservice.h"

CountService::CountService(QObject* parent)
    : QObject(parent)
    , count(0)
{
}

int CountService::currentCount() const
{
    return this->count;
}

void CountService::increment()
{
    this->count += 1;
    emit countChanged();
}
