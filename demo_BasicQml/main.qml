import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import DIQt 1.0

Window {
    id: window
    width: 320
    height: 480
    visible: true

    Item {
        id: root
        anchors.fill: parent

        // QML依赖注入格式：property [类型] di_consumer_[自定义变量名]
        property CountService di_consumer_countService;

        // 所有属性注入完毕后会调用名为di_onInit和di_onInit_[自定义名称]的函数
        function di_onInit() {
            console.log("init");
            console.log(di_consumer_countService);
        }

        Button {
            width: 120
            height: 60
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 40
            text: 'Execute'
            font.pointSize: 12

            onPressed: {
                // 调用 Service 中的函数前，先判断是否完成注入，即 Service 对象不为 null
                if (!root.di_consumer_countService) {
                    return;
                }
                // 可直接调用 Service 中的 Q_INVOKABLE 函数或槽函数
                root.di_consumer_countService.increment();
            }
        }

        Text {
            id: text
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -40
            text: '0'
            font.pointSize: 24

            // 使用 Connections 连接 Service 中的信号
            // 第一行 target 固定为 Service 对象
            // 第二行写法依照信号名，如信号为 countChanged，则第二行为 onCountChanged。注意大小写。
            Connections {
                target: root.di_consumer_countService
                onCountChanged: {
                    // Connections 当 target 为 null 时无效，因此不需要对注入完成与否另行判断
                    text.text = root.di_consumer_countService.currentCount()
                }
            }
        }
    }
}
