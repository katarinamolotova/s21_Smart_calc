#ifndef SRC_VIEW_S21_VIEW_H_
#define SRC_VIEW_S21_VIEW_H_

#include <QMainWindow>
#include <string>
#include <QDebug>
#include <iostream>
#include <QValidator>
#include "qcustomplot.h"
#include <QVector>
#include <limits>
#include <limits.h>
#include <float.h>
#include "credite_calc_v2/s21_credit_calc.h"
#include "deposite_calc_v2/s21_depos_calc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class s21_view; }
QT_END_NAMESPACE

class s21_view : public QMainWindow {
    Q_OBJECT

 private:
    Ui::s21_view *ui;
    credit_calc *credit_window;
    depos_calc *depos_window;

 public:
    s21_view(QWidget *parent = nullptr);
    ~s21_view();
    s21::Controller *Control;

 private slots:

    void validator();
    void keys_setting();

    void digits_numbers();
    void backspace_delete();
    void trigonometry();
    void calculate();
    void check_radio();

    std::pair<double, double> radio_res();
    void visibleView(int x, int y);

    void makeGraph(std::pair<double, double> dataX);
    void setRangeGraph(double x, double y);
    void putSizeX();

    void putSizeY(double resultY, double * minY, double * maxY);
    double setStep();

    void financial();
};
#endif  //  SRC_VIEW_S21_VIEW_H_
