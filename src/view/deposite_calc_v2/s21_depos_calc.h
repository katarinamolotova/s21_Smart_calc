//#ifndef SRC_DEPOS_CALC_H
//#define SRC_DEPOS_CALC_H

#include <QDialog>
#include "../controller/s21_controller.h"

namespace Ui {
    class depos_calc;
}

class depos_calc : public QDialog {
    Q_OBJECT

signals:
    void s21_view();

public:
    explicit depos_calc(QWidget *parent = nullptr);
    s21::Controller *control;
    std::vector<std::pair<std::string, std::pair<int, double>>> additions;
    std::vector<std::pair<std::string, std::pair<int, double>>> withdrawal;
    ~depos_calc();

private slots:
    void validatorSetting();
    void on_run_clicked();
    void on_add_add_clicked();
    void on_del_add_clicked();
    void on_sub_add_clicked();
    void on_del_sub_clicked();
    void back();
    int set_value_for_capitalis_and_period(int value);

private:
    Ui::depos_calc *ui;
};

//#endif // SRC_DEPOS_CALC_H
