#ifndef SRC_VIEW_CREDITE_CALC_V2_CREDIT_CALC_H
#define SRC_VIEW_CREDITE_CALC_V2_CREDIT_CALC_H

#include <QDialog>
#include "../../controller/s21_controller.h"

namespace Ui {
class credit_calc;
}

class credit_calc : public QDialog {
    Q_OBJECT

signals:
    void s21_view();

public:
    explicit credit_calc(QWidget *parent = nullptr);
    ~credit_calc();
    s21::Controller *Control;

private slots:
    void back();
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();
    void validatorSetting();
    void checkData();
    void calculate();
    void checkProcent();
    void checkPeriod();
    void checkSum();
    void print(std::vector<double> result, std::string Sum);

private:
    Ui::credit_calc *ui;
};

#endif // SRC_VIEW_CREDITE_CALC_V2_CREDIT_CALC_H
