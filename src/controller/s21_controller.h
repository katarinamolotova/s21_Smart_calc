#ifndef SRC_CONTROLLER_S21_CONTROLLER_H_
#define SRC_CONTROLLER_S21_CONTROLLER_H_

#include "../model/s21_model.h"

namespace s21 {

class Model;

class Controller {
 private:
    Model *model;

 public:
    Controller() {
       model = new Model();
    }
    ~Controller() {
       delete model;
    }

    //  польская нотация
    pair<string, int> calculate(string expression, double x);

    //  кредит
    std::vector<double> every_moth_pay(int type, string sum,
       pair<string, int> amount_of_month, string procent);
    double total_payment(std::vector<double> every_month);
    double overpay(double total_payment, string sum);

    // депозит
    bool check_list_add_and_sub(std::vector<pair<string, pair<int, double>>> list);
    double result_procent(string sum, pair<string, int> amount_of_month,
       string procent, int capitaliz, int period_pay, string month_start,
       std::vector<pair<string, pair<int, double>>> &additions,
       std::vector<pair<string, pair<int, double>>> &withdrawal);
    void check_add_list(int& add_index, int index, double& add,
       std::vector<pair<string, pair<int, double>>> &additions);
    void check_sub_list(int& sub_index, int index, double& sub,
       std::vector<pair<string, pair<int, double>>> &withdrawal);
    void check_capitaliz(int& count_cap, int capitaliz, double& temp_procent,
       double& intermediate_sum);
    void check_period_pay(int& count_pay, int period_pay, double& intermediate_sum,
       string sum);
    double sum_at_the_end(string sum_begin, double result_procent,
       std::vector<pair<string, pair<int, double>>> &additions,
       std::vector<pair<string, pair<int, double>>> &withdrawal);
    double sum_nalog(string nalog_procent, double result_procent);
};
}  // namespace s21

#endif  // SRC_CONTROLLER_S21_CONTROLLER_H_
