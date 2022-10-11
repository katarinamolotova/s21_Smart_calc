#include "s21_controller.h"

using namespace s21;

pair<string, int> Controller::calculate(string expression, double x = 0) {
    pair<string, int> result;
    try {
        model->preparing_for_parcer(expression, std::to_string(x));
        model->parser(expression);
        model->calculate();
        result = std::make_pair(std::to_string(model->answer_), 1);
    } catch (std::invalid_argument& e) {
        model->clear();
        result = std::make_pair(e.what(), 0);
    }
    return result;
}

//  credit
std::vector<double> Controller::every_moth_pay(int type, string sum,
      pair<string, int> amount_of_month, string procent) {
    std::vector<double> every_month;
    const double sum_stat = stod(sum);
    double sum_dynam = stod(sum);
    int data = (amount_of_month.second != 0) ? stoi(amount_of_month.first) * 12 : stoi(amount_of_month.first);
    string expression;
    for (auto i = 0; i < data; i++) {
        if (type == 1)
            expression = std::to_string(sum_stat) + "*((" + procent +
                "/100/12)/(1-(1+(" + procent + "/100/12))^(-1*" + std::to_string(data) + ")))";
        else
            expression = std::to_string(sum_stat) + "/" + std::to_string(data) +
                "+" + std::to_string(sum_dynam) + "*(" + procent + "/100/12)";
        model->parser(expression);
        model->calculate();
        every_month.push_back(model->answer_);
        sum_dynam -= every_month[i];
    }
    return every_month;
}

double Controller::total_payment(std::vector<double> every_month) {
    double temp = every_month[0];
    for (size_t i = 1; i < every_month.size(); i++) {
        string expression = std::to_string(temp) + "+" + std::to_string(every_month[i]);
        model->parser(expression);
        model->calculate();
        temp = model->answer_;
    }
    return model->answer_;
}

double Controller::overpay(double total_payment, string sum) {
    string expression = std::to_string(total_payment) + "-" + sum;
    model->parser(expression);
    model->calculate();
    return model->answer_;
}

//  deposit
bool Controller::check_list_add_and_sub(std::vector<pair<string, pair<int, double>>> list) {
    bool result = true;
    try {
        model->check_for_deposit(list);
    } catch (std::invalid_argument& e) {
        result = false;
    }
    return result;
}

double Controller::result_procent(string sum, pair<string, int> amount_of_month,
        string procent, int capitaliz, int period_pay, string month_start,
        std::vector<pair<string, pair<int, double>>> &additions,
        std::vector<pair<string, pair<int, double>>> &withdrawal) {
    double result = 0, intermediate_sum = stod(sum), temp_procent = 0;
    int sub_index = 0, add_index = 0;
    double add = 0, sub = 0;
    int data = (amount_of_month.second != 0) ? stoi(amount_of_month.first) * 12 : stoi(amount_of_month.first);
    for (int i = stoi(month_start), count_cap = 0, count_pay = 0;
            i < stoi(month_start) + data; i++, count_cap++, count_pay++) {
        int index = (i + 11) % 12;
        int days_in_month = (index == 1) ? 28 : (31 - index % 7 % 2);
        check_add_list(add_index, index, add, additions);
        check_sub_list(sub_index, index, sub, withdrawal);
        check_capitaliz(count_cap, capitaliz, temp_procent, intermediate_sum);
        check_period_pay(count_pay, period_pay, intermediate_sum, sum);
        string expression = "(" + std::to_string(intermediate_sum) + "+" + std::to_string(add)
            + "-" + std::to_string(sub) + ")/100*" + procent + "*" + std::to_string(days_in_month) + "/365";
        model->parser(expression);
        model->calculate();
        result += model->answer_;
        temp_procent += model->answer_;
    }
    return result;
}

void Controller::check_add_list(int& add_index, int index, double& add,
        std::vector<pair<string, pair<int, double>>> &additions) {
    if (add_index < static_cast<int>(additions.size()) && index == additions[add_index].second.first) {
        add += additions[add_index].second.second;
        add_index++;
    }
}

void Controller::check_sub_list(int& sub_index, int index, double& sub,
        std::vector<pair<string, pair<int, double>>> &withdrawal) {
    if (sub_index < static_cast<int>(withdrawal.size()) && index == withdrawal[sub_index].second.first) {
        sub += withdrawal[sub_index].second.second;
        sub_index++;
    }
}

void Controller::check_capitaliz(int& count_cap, int capitaliz, double& temp_procent,
         double& intermediate_sum) {
    if (count_cap == capitaliz && capitaliz) {
        intermediate_sum += temp_procent;
        count_cap = 0;
        temp_procent = 0;
    }
}

void Controller::check_period_pay(int& count_pay, int period_pay, double& intermediate_sum, string sum) {
    if (count_pay == period_pay && period_pay) {
        intermediate_sum = stod(sum);
        count_pay = 0;
    }
}

double Controller::sum_at_the_end(string sum_begin, double result_procent,
        std::vector<pair<string, pair<int, double>>> &additions,
        std::vector<pair<string, pair<int, double>>> &withdrawal) {
    double result = 0;
    string expression = sum_begin + "+" + std::to_string(result_procent);
    model->parser(expression);
    model->calculate();
    result = model->answer_;
    for (auto it = additions.begin(); it != additions.end(); it++)
        result += it->second.second;
    for (auto it = withdrawal.begin(); it != withdrawal.end(); it++)
        result -= it->second.second;
    return result;
}

double Controller::sum_nalog(string nalog_procent, double result_procent) {
    string expression = std::to_string(result_procent) + "*(" + nalog_procent + "/100)";
    model->parser(expression);
    model->calculate();
    return model->answer_;
}
