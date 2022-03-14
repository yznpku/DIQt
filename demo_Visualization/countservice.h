#ifndef COUNTSERVICE_H
#define COUNTSERVICE_H

#include <QMetaType>
#include <QObject>

class CountService : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE CountService(QObject* parent = 0);

    int currentCount() const;
    void increment();

signals:
    void countChanged();

private:
    int count;
};

Q_DECLARE_METATYPE(CountService*)

#endif // COUNTSERVICE_H
