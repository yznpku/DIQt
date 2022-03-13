#ifndef COUNTSERVICE_H
#define COUNTSERVICE_H

#include <QObject>

class CountService : public QObject {
    Q_OBJECT

public:
    Q_INVOKABLE CountService(QObject* parent = nullptr);

    // 需要在 QML 中调用的函数均需添加 Q_INVOKABLE，或定义为槽函数
    Q_INVOKABLE int currentCount() const;
    Q_INVOKABLE void increment();

signals:
    void countChanged();

private:
    int count { 0 };
};

#endif // COUNTSERVICE_H
