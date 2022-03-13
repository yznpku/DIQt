/*
 * Copyright (c) 2019 Zhining Yang. <yzn20080@gmail.com>
 */

#include <DIQt>
#include <QtCore>
#include <QtDebug>
#include <QtTest>

class ServiceA : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE ServiceA(QObject* parent = 0)
        : QObject(parent)
    {
    }
};
Q_DECLARE_METATYPE(ServiceA*)

class ServiceB : public QObject {
    Q_OBJECT

    ServiceA* serviceA;

public:
    Q_INVOKABLE ServiceB(QObject* parent = 0)
        : QObject(parent)
        , serviceA(0)
    {
    }

    ServiceA* getServiceA() const
    {
        return this->serviceA;
    }

public slots:
    DI_CONSUMER void setServiceA(ServiceA* serviceA)
    {
        this->serviceA = serviceA;
    }
};
Q_DECLARE_METATYPE(ServiceB*)

class WidgetC : public QObject {
    Q_OBJECT

    ServiceB* serviceB;

public:
    Q_INVOKABLE WidgetC(QObject* parent = 0)
        : QObject(parent)
        , serviceB(0)
    {
    }

    ServiceB* getServiceB() const
    {
        return this->serviceB;
    }

public slots:
    DI_CONSUMER void setServiceB(ServiceB* serviceB)
    {
        this->serviceB = serviceB;
    }
};
Q_DECLARE_METATYPE(WidgetC*)

class WidgetD : public QObject {
    Q_OBJECT

public:
    WidgetD(QObject* parent = 0)
        : QObject(parent)
    {
        c1 = new WidgetC(this);
        c2 = new WidgetC(this);
    }

    WidgetC* c1;
    WidgetC* c2;
};
Q_DECLARE_METATYPE(WidgetD*)

/**
 * @brief
 * 测试不同的provide配置，注入结果是否符合预期。
 * 设定依赖关系：WidgetC -> ServiceB, ServiceB -> ServiceA
 */
class TestMultipleTrees : public QObject {
    Q_OBJECT

private slots:

    /**
     * @brief
     * 将ServiceA提供给ServiceB，将ServiceB提供给WidgetC，并bootstrap两个WidgetC实例。
     * 由于ServiceB被提供给WidgetC，因此不同的WidgetC实例将注入不同的ServiceB实例。
     * 由于ServiceA被提供给ServiceB，因此不同的ServiceB实例将注入不同的ServiceA实例。
     */
    void provide_A_to_B_and_B_to_C()
    {
        DIQtModule module;
        module.provide<WidgetC, ServiceB>();
        module.provide<ServiceB, ServiceA>();

        WidgetC widgetC1, widgetC2;
        module.bootstrap(&widgetC1);
        module.bootstrap(&widgetC2);

        // 测试注入是否成功
        QVERIFY2(widgetC1.getServiceB(), "将B注入C1成功");
        QVERIFY2(widgetC1.getServiceB()->getServiceA(), "将A注入C1.B成功");
        QVERIFY2(widgetC2.getServiceB(), "将B注入C2成功");
        QVERIFY2(widgetC2.getServiceB()->getServiceA(), "将A注入C1.B成功");

        // 测试两个WidgetC是否注入了不同的ServiceB实例
        QVERIFY2(widgetC1.getServiceB() != widgetC2.getServiceB(), "两个WidgetC实例注入不同的ServiceB实例");

        // 测试两个WidgetC是否注入了不同的ServiceA实例
        QVERIFY2(widgetC1.getServiceB()->getServiceA() != widgetC2.getServiceB()->getServiceA(), "两个WidgetC实例注入不同的ServiceA实例");
    }

    /**
     * @brief
     * 将ServiceA在根部提供，将ServiceB提供给WidgetC，并bootstrap两个WidgetC实例。
     * 由于ServiceB被提供给WidgetC，因此不同的WidgetC实例将注入不同的ServiceB实例。
     * 由于ServiceA在根部提供，因此不同的ServiceB实例将注入相同的ServiceA实例。
     */
    void provide_A_at_root_and_B_to_C()
    {
        DIQtModule module;
        module.provide<WidgetC, ServiceB>();
        module.provideAtRoot<ServiceA>();

        WidgetC widgetC1, widgetC2;
        module.bootstrap(&widgetC1);
        module.bootstrap(&widgetC2);

        // 测试注入是否成功
        QVERIFY2(widgetC1.getServiceB(), "将B注入C1成功");
        QVERIFY2(widgetC1.getServiceB()->getServiceA(), "将A注入C1.B成功");
        QVERIFY2(widgetC2.getServiceB(), "将B注入C2成功");
        QVERIFY2(widgetC2.getServiceB()->getServiceA(), "将A注入C1.B成功");

        // 测试两个WidgetC是否注入了不同的ServiceB实例
        QVERIFY2(widgetC1.getServiceB() != widgetC2.getServiceB(), "两个WidgetC实例注入不同的ServiceB实例");

        // 测试两个WidgetC是否注入了相同的ServiceA实例
        QVERIFY2(widgetC1.getServiceB()->getServiceA() == widgetC2.getServiceB()->getServiceA(), "两个WidgetC实例注入相同的ServiceA实例");
    }

    /**
     * @brief
     * 将ServiceA提供给WidgetD，将ServiceB提供给WidgetC，并bootstrap两个WidgetC实例。
     * 由于ServiceB被提供给WidgetC，因此不同的WidgetC实例将注入不同的ServiceB实例。
     * 由于ServiceA被提供给WidgetD，因此同一WidgetD实例中的两个的ServiceB实例将注入相同的ServiceA实例。
     */
    void provide_A_to_D_and_B_to_C()
    {
        DIQtModule module;
        module.provide<WidgetC, ServiceB>();
        module.provide<WidgetD, ServiceA>();

        WidgetD widgetD;
        module.bootstrap(&widgetD);

        // 测试注入是否成功
        QVERIFY2(widgetD.c1->getServiceB(), "将B注入C1成功");
        QVERIFY2(widgetD.c1->getServiceB()->getServiceA(), "将A注入C1.B成功");
        QVERIFY2(widgetD.c2->getServiceB(), "将B注入C2成功");
        QVERIFY2(widgetD.c2->getServiceB()->getServiceA(), "将A注入C1.B成功");

        // 测试两个WidgetC是否注入了不同的ServiceB实例
        QVERIFY2(widgetD.c1->getServiceB() != widgetD.c2->getServiceB(), "两个WidgetC实例注入不同的ServiceB实例");

        // 测试两个WidgetC是否注入了相同的ServiceA实例
        QVERIFY2(widgetD.c1->getServiceB()->getServiceA() == widgetD.c2->getServiceB()->getServiceA(), "两个WidgetC实例注入相同的ServiceA实例");
    }
};

QTEST_MAIN(TestMultipleTrees)

#include "main.moc"
