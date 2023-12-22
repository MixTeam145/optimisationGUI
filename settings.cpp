#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
    , niter(100)
    , eps(1e-3)
    , prob(0.6)
    , delta(1)
    , alpha(1)
    , seed(std::chrono::system_clock::now().time_since_epoch().count())
    , dim(2)
    , min_point(2)
    , max_point(2)
    , start_point(2)
{
    min_point << -1.5, -3;
    max_point << 4, 4;
    start_point << -1, -1;
    f = function::McCormick;

    ui->setupUi(this);

    ui->stackedWidgetParams1->setCurrentIndex(0);
    ui->stackedWidgetParams2->setCurrentIndex(0);
    ui->stackedDim->setCurrentIndex(0);

    ui->listFunctions->addItem("McCormick: f(x) = sin(x[1] + x[2]) + (x[1] - x[2])^2 - 1.5x[1] + 2.5x[2] + 1");
    ui->listFunctions->addItem("Booth: f(x) = (x[1] + 2x[2] - 7)^2 + (2x[1] + x[2] - 5)^2");
    ui->listFunctions->addItem("Rosenbrock: f(x) = 100(x[2]-x[1]^2)^2 + (1 - x[1])^2 + ...");

    ui->listFunctions->setCurrentRow(0);

    for (size_t i = 0; i < dim; ++i) {
        ui->listDomain->addItem(std::format("x{}: [{}, {}]", i + 1, min_point[i], max_point[i]).c_str());
        ui->listStartPoint->addItem(std::format("x{}: {}", i + 1, start_point[i]).c_str());
    }

    ui->radioButtonMethod->click();
    ui->radioButtonStopper->click();

    ui->editDim->setText(std::format("{}", dim).c_str());
    ui->editNiter->setText(std::format("{}", niter).c_str());
    ui->editEps->setText(std::format("{}", eps).c_str());
    ui->editProb->setText(std::format("{}", prob).c_str());
    ui->editDelta->setText(std::format("{}", delta).c_str());
    ui->editAlpha->setText(std::format("{}", alpha).c_str());
    ui->editSeed->setText(std::format("{}", seed).c_str());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_editDim_textEdited(const QString &arg1)
{
    dim = arg1.toUInt();
    min_point.resize(dim);
    max_point.resize(dim);
    start_point.resize(dim);
    ui->listDomain->clear();
    ui->listStartPoint->clear();
    for (Index i = 0; i < min_point.size(); ++i) {
        min_point[i] = -2;
        max_point[i] = 2;
        start_point[i] = 0;
        ui->listDomain->addItem(std::format("x{}: [{}, {}]", i + 1, min_point[i], max_point[i]).c_str());
        ui->listStartPoint->addItem(std::format("x{}: {}", i + 1, start_point[i]).c_str());
    }
}

void Settings::on_radioButtonMethod_clicked()
{
    ui->stackedWidgetParams2->setCurrentIndex(0);
    m = method::Deterministic;
}

void Settings::on_radioButtonMethod_2_clicked()
{
    ui->stackedWidgetParams2->setCurrentIndex(1);
    m = method::Stochastic;
}

void Settings::on_radioButtonStopper_clicked()
{
    ui->stackedWidgetParams1->setCurrentIndex(1);
    s = stopper::Abs;
}

void Settings::on_radioButtonStopper_2_clicked()
{
    ui->stackedWidgetParams1->setCurrentIndex(0);
    s = stopper::Number;
}

void Settings::on_listStartPoint_itemClicked(QListWidgetItem *item)
{
    Index ind = ui->listStartPoint->currentRow();
    ui->editStart->setText(std::format("{}", start_point[ind]).c_str());
}

void Settings::on_editStart_textEdited(const QString &arg1)
{
    Index ind = ui->listStartPoint->currentRow();
    start_point[ind] = arg1.toDouble();
    ui->listStartPoint->currentItem()->setText(std::format("x{}: {}", ind + 1, start_point[ind]).c_str());
}

void Settings::on_listDomain_itemClicked(QListWidgetItem *item)
{
    Index ind = ui->listDomain->currentRow();
    ui->editMin->setText(std::format("{}", min_point[ind]).c_str());
    ui->editMax->setText(std::format("{}", max_point[ind]).c_str());
}

void Settings::on_editMin_textEdited(const QString &arg1)
{
    Index ind = ui->listDomain->currentRow();
    min_point[ind] = arg1.toDouble();
    ui->listDomain->currentItem()->setText(std::format("x{}: [{}, {}]",
                                                       ind + 1,
                                                       min_point[ind],
                                                       max_point[ind]).c_str());
}

void Settings::on_editMax_textEdited(const QString &arg1)
{
    Index ind = ui->listDomain->currentRow();
    max_point[ind] = arg1.toDouble();
    ui->listDomain->currentItem()->setText(std::format("x{}: [{}, {}]",
                                                       ind + 1,
                                                       min_point[ind],
                                                       max_point[ind]).c_str());
}

bool Settings::check_input()
{
    long long num;
    double x;
    std::string warnings = "";

    num = ui->editNiter->text().toLongLong();
    if (num <= 0 || num > 1000000)
        warnings += "Max number of iterations must be > 0 and <= 1000000.\n";

    x = ui->editEps->text().toDouble();
    if (x <= 0)
        warnings += "Precision must be > 0.\n";

    x = ui->editAlpha->text().toDouble();
    if (x <= 0 || x > 1)
        warnings += "Alpha must be > 0 and <= 1.\n";

    x = ui->editDelta->text().toDouble();
    if (x <= 0)
        warnings += "Delta must be > 0.\n";

    x = ui->editProb->text().toDouble();
    if (x <= 0 || x > 1)
        warnings += "Probability must be > 0 and <= 1.\n";

    for (Index i = 0; i < min_point.size(); ++i) {
        if (min_point[i] > max_point[i])
            warnings += std::format("Min[{0}] must be < Max[{0}]\n", i + 1);
        else if (start_point[i] < min_point[i] || start_point[i] > max_point[i])
            warnings += std::format("Start point[{0}] must be >= min[{0}] and <= max[{0}]", i + 1);
    }

    if (warnings == "")
        return true;
    else {
        QMessageBox::warning(this, "Warning!", warnings.c_str());
        return false;
    }
}

void Settings::apply_changes()
{
    niter = ui->editNiter->text().toUInt();
    eps = ui->editEps->text().toDouble();
    prob = ui->editProb->text().toDouble();
    delta = ui->editDelta->text().toDouble();
    alpha = ui->editAlpha->text().toDouble();
    seed = ui->editSeed->text().toUInt();
    dim = ui->editDim->text().toUInt();
}

void Settings::on_listFunctions_itemClicked(QListWidgetItem *item)
{
    ui->listDomain->clear();
    ui->listStartPoint->clear();
    min_point.resize(2);
    max_point.resize(2);
    start_point.resize(2);
    switch (ui->listFunctions->currentRow()) {
    case 0:
        f = function::McCormick;
        ui->listDomain->addItem("x1: [-1.5, 4]");
        ui->listDomain->addItem("x2: [-3, 4]");
        ui->listStartPoint->addItem("x1: -1");
        ui->listStartPoint->addItem("x2: -1");
        min_point << -1.5, -3;
        max_point << 4, 4;
        start_point << -1, -1;
        ui->stackedDim->setCurrentIndex(0);
        break;
    case 1:
        f = function::Booth;
        ui->listDomain->addItem("x1: [-10, 10]");
        ui->listDomain->addItem("x2: [-10, 10]");
        ui->listStartPoint->addItem("x1: 0");
        ui->listStartPoint->addItem("x2: 0");
        min_point << -10, -10;
        max_point << 10, 10;
        start_point << 0, 0;
        ui->stackedDim->setCurrentIndex(0);
        break;
    case 2:
        f = function::Rosenbrock;
        ui->listDomain->addItem("x1: [-2, 2]");
        ui->listDomain->addItem("x2: [-2, 2]");
        ui->listStartPoint->addItem("x1: 0");
        ui->listStartPoint->addItem("x2: 0");
        min_point << -2, -2;
        max_point << 2, 2;
        start_point << 0, 0;
        ui->stackedDim->setCurrentIndex(1);
        break;
    default:
        break;
    }
    ui->editDim->setText("2");
    ui->editMin->clear();
    ui->editMax->clear();
    ui->editStart->clear();
}

void Settings::done(int r)
{
    if (r == QDialog::Accepted) {
        if (check_input()) {
            apply_changes();
             QDialog::done(r);
        }
    }
    else QDialog::done(r);
}

