#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include "Eigen/Dense"

using namespace Eigen;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

    ~Settings();

    enum class method
    {
        Deterministic,
        Stochastic
    };

    enum class stopper
    {
        Abs,
        Number
    };

    enum class function
    {
        McCormick,
        Booth,
        Rosenbrock
    };

    size_t get_dim() { return dim; }

    VectorXd get_min_point() { return min_point; }

    VectorXd get_max_point() { return max_point; }

    VectorXd get_start_point() {return start_point; }

    size_t get_niter() { return niter; }

    double get_eps() { return eps; }

    unsigned int get_seed() { return seed; }

    double get_prob() { return prob; }

    double get_delta() { return delta; }

    double get_alpha() { return alpha; }

    method get_method() { return m; }

    stopper get_stopper() { return s; }

    function get_function() { return f; }

    void set_seed(unsigned new_seed = std::chrono::system_clock::now().time_since_epoch().count()) { seed = new_seed; }
private slots:
    void on_editDim_textEdited(const QString &arg1);

    void on_radioButtonMethod_clicked();

    void on_radioButtonMethod_2_clicked();

    void on_radioButtonStopper_clicked();

    void on_radioButtonStopper_2_clicked();

    void on_listStartPoint_itemClicked(QListWidgetItem *item);

    void on_editStart_textEdited(const QString &arg1);

    void on_listDomain_itemClicked(QListWidgetItem *item);

    void on_editMin_textEdited(const QString &arg1);

    void on_editMax_textEdited(const QString &arg1);

    void on_listFunctions_itemClicked(QListWidgetItem *item);

    bool check_input();

    void apply_changes();

    void done(int r) override;

private:
    Ui::Settings *ui;


    method m;
    stopper s;
    function f;

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
};

#endif // SETTINGS_H
