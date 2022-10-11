#ifndef SRC_MODEL_S21_MODEL_H_
#define SRC_MODEL_S21_MODEL_H_

#include <queue>
#include <stack>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <ctype.h>

using std::string;
using std::pair;
using std::stack;

namespace s21 {

class Model {
    friend class Controller;
 private:
    std::queue<pair<string, double>> polish_notation_;
    double answer_;

    Model() {}

    void clear();

    void preparing_for_parcer(string& str, string value);
    void replace_x(string& str, string value);
    void insert_multiple(string& str);

    void parser(string str);

    void prioritization(stack<string>& operations, string op);
    int priority(string op);

    int shift_operation(string op);
    int shift_number(string value);

    void get_value_in_brackets(stack<string>& operations);
    string get_operations(string str);

    void numbers_processing(size_t& index, string str, bool& is_unary);
    void operations_processing(stack<string>& operations, size_t& index,
        string str, bool& is_unary);


    void calculate();

    void check_sqrt(double value);
    void check_division(double value);
    void check_str(string str);

    bool is_binary_operation(string op);
    bool is_unary_operation(string op);

    void perfoming_binary_operation(stack<double>& intermediate_result,
        string operation);
    void perfoming_unary_operation(stack<double>& intermediate_result,
        string operation);

    double get_value_from_stack(stack<double>& intermediate_result);

    void check_for_deposit(std::vector<pair<string, pair<int, double>>> list);
};

}  // namespace s21

#endif  // SRC_MODEL_S21_MODEL_H_
