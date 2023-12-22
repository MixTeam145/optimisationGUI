#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DeterministicOptimizer.h"
#include "StochasticOptimizer.h"
#include "BoothFunction.h"
#include "McCormickFunction.h"
#include "RosenbrockFunction.h"
#include "AbsValueSC.h"
#include "NumIterSC.h"
#include "settings.h"
#include "mygraphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void plot_trajectory(MyGraphicsScene* scene);

    void plot_2d();

private slots:
    void on_actionSettings_triggered();

    void on_actionOptimize_triggered();

    void on_scene_clicked(QPointF point);

    void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow *ui;

    Settings dial;
    IOFormat fmt;

    size_t niter;
    double eps;
    double prob;
    double delta;
    double alpha;
    unsigned int seed;
    size_t dim;
    VectorXd min_point;
    VectorXd max_point;
    VectorXd start_point;
    Function* f;
    StopCriterion* criterion;
    Optimizer* optimizer;

    std::vector<VectorXd> trajectory;

    MyGraphicsScene* scene;
};
#endif // MAINWINDOW_H
