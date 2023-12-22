#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fmt(StreamPrecision, 0, ", ", ", ", "", "", "(", ")")
    , niter(100)
    , eps(1e-3)
    , prob(0.6)
    , delta(1)
    , alpha(0.5)
    , seed(0)
    , dim(2)
    , min_point(dim)
    , max_point(dim)
    , start_point(dim)
    , f(new McCormickFunction)
    , criterion(new AbsValueSC(niter, eps))
    , optimizer(new DeterministicOptimizer(f, criterion))
    , trajectory()
    , scene(nullptr)
{
    ui->setupUi(this);
    min_point << -1.5, -3;
    max_point << 4, 4;
    start_point << -1, -1;
    ui->graphicsOptim->setRenderHints(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete optimizer;
    delete criterion;
    delete f;
    delete ui;
}

void MainWindow::plot_trajectory(MyGraphicsScene* scene)
{
    int plot_width = ui->graphicsOptim->width(),
        plot_height = ui->graphicsOptim->height();
    double area_width = max_point[0] - min_point[0],
        area_height = max_point[1] - min_point[1];
    QPen pen(QColor(255, 0, 0));
    for (size_t i = 1; i < trajectory.size(); ++i) {
        scene->addLine(
            (trajectory[i - 1][0] - min_point[0]) * plot_width / area_width,
            (max_point[1] - trajectory[i - 1][1]) * plot_height / area_height,
            (trajectory[i][0] - min_point[0]) * plot_width / area_width,
            (max_point[1] - trajectory[i][1]) * plot_height / area_height, pen
            );
    }
}

void MainWindow::plot_2d()
{
    if (scene) delete scene;
    scene = new MyGraphicsScene(ui->graphicsOptim);
    QObject::connect(scene, &MyGraphicsScene::sceneClicked, this, &MainWindow::on_scene_clicked);

    int size_rect = 10;

    int scene_width = ui->graphicsOptim->width();
    int scene_height = ui->graphicsOptim->height();
    int width = scene_width / size_rect;
    int height = scene_height / size_rect;

    double grid_step_x = (max_point[0] - min_point[0]) / width;
    double grid_step_y = (max_point[1] - min_point[1]) / height;

    double min_value = std::numeric_limits<double>::infinity(),
        max_value = -min_value;

    double value;
    VectorXd point(2);
    for (int i = 0; i <= width; ++i) {
        for (int j = 0; j <= height; ++j) {
            point << min_point[0] + i * grid_step_x,
                min_point[1] + j * grid_step_y;
            value = f->eval(point);
            max_value = std::max(max_value, value);
            min_value = std::min(min_value, value);
        }
    }

    double max_value_trunc = std::min(max_value, min_value + 255);
    int blue, green;
    for (int i = 0; i <= width; ++i) {
        for (int j = 0; j <= height; ++j) {
            point << min_point[0] + i * grid_step_x,
                min_point[1] + j * grid_step_y;
            value = f->eval(point);
            if (value < max_value_trunc) {
                blue = (value - min_value) / (max_value_trunc - min_value) * 255;
                green = 0;
            }
            else {
                double proportion = (max_value - value) / (max_value - max_value_trunc);
                blue = proportion * 255;
                green = (1 - proportion) * 255;
            }
            QPen pen(QColor(0, green, blue));
            QBrush brush(QColor(0, green, blue));
            scene->addRect(size_rect * i, scene_height - j * size_rect, size_rect, size_rect, pen, brush);
        }
    }

    if (!optimizer->get_trajectory().empty())
        plot_trajectory(scene);

    ui->graphicsOptim->setScene(scene);
}

void MainWindow::on_actionSettings_triggered()
{
    if (dial.exec() == QDialog::Accepted) {
        dim = dial.get_dim();
        if (f) delete f;
        switch (dial.get_function()) {
        case Settings::function::McCormick:
            f = new McCormickFunction();
            break;
        case Settings::function::Booth:
            f = new BoothFunction();
            break;
        case Settings::function::Rosenbrock:
            f = new RosenbrockFunction(dim);
            break;
        }
        niter = dial.get_niter();
        eps = dial.get_eps();
        if (criterion) delete criterion;
        switch (dial.get_stopper()) {
        case Settings::stopper::Number:
            criterion = new NumIterSC(niter);
            break;
        case Settings::stopper::Abs:
            criterion = new AbsValueSC(niter, eps);
            break;
        }
        prob = dial.get_prob();
        delta = dial.get_delta();
        alpha = dial.get_alpha();
        seed = dial.get_seed();
        if (optimizer) delete optimizer;
        switch (dial.get_method()) {
        case Settings::method::Deterministic:
            optimizer = new DeterministicOptimizer(f, criterion);
            break;
        case Settings::method::Stochastic:
            optimizer = new StochasticOptimizer(f, criterion, prob, delta, alpha, seed);
            break;
        }
        min_point = dial.get_min_point();
        max_point = dial.get_max_point();
        start_point = dial.get_start_point();

        if (dim == 2)
            plot_2d();

        else {

            ui->graphicsOptim->scene()->clear();
        }

        ui->labelFuncVal->clear();
        ui->labelLocMin->clear();
        ui->labelNiter->clear();
        ui->labelStart->clear();
    }
}

void MainWindow::on_actionOptimize_triggered()
{
    optimizer->set_domain(min_point, max_point);
    optimizer->optimize(start_point);
    trajectory = optimizer->get_trajectory();

    std::stringstream ss;
    ui->labelStart->setText((ss << start_point.format(fmt), ss.str().c_str()));
    ui->labelLocMin->setText((ss.str(""), ss << trajectory.back().format(fmt), ss.str().c_str()));
    ui->labelNiter->setText(QString::number(trajectory.size() - 1));
    ui->labelFuncVal->setText(QString::number(f->eval(trajectory.back())));

    if (dim == 2)
        plot_2d();
}

void MainWindow::on_scene_clicked(QPointF point)
{
    if (dim == 2)
    {
        int plot_width = ui->graphicsOptim->width();
        int plot_height = ui->graphicsOptim->height();

        double grid_step_x = (max_point[0] - min_point[0]) / plot_width;
        double grid_step_y = (max_point[1] - min_point[1]) / plot_height;

        double x = min_point[0] + grid_step_x * point.x();
        double y = max_point[1] - grid_step_y * point.y();

        RectArea area(min_point, max_point);
        if (area.is_inside(Vector2d(x, y))) {
            start_point = Vector2d(x, y);
            on_actionOptimize_triggered();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (dim == 2)
        plot_2d();
}
