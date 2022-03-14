#ifndef DIQTVISUALIZER_H
#define DIQTVISUALIZER_H

#include <QWidget>

class DIQtModule;
class DIQtVisualizerPrivate;
class DIQtVisualizer : public QWidget {
    Q_OBJECT

public:
    DIQtVisualizer(QWidget* parent = nullptr);
    ~DIQtVisualizer();

    void visualize(QObject* object);
    void visualize(DIQtModule* module);

private:
    DIQtVisualizerPrivate* d;
};

#endif // DIQTVISUALIZER_H
