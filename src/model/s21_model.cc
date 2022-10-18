#include "s21_model.h"

using namespace s21;

void Model::preparing_for_parcer(string& str, string value) {
    check_str(str);
    replace_x(str, value);
    insert_multiple(str);
}

void Model::replace_x(string& str, string value) {
    size_t pos;
    string replace = "()";
    replace.insert(1, value);
    while ((pos = str.find("x")) != string::npos) {
        size_t prev_pos = pos - 1, next_pos = pos + 1;
        if (isdigit(str[prev_pos])) {
            str.insert(pos, "*");
            pos++;
        } else if (isdigit(str[next_pos])) {
            str.insert(next_pos, "*");
        }
        str.replace(pos, 1, replace);
    }
}

void Model::insert_multiple(string& str) {
    string search = "(cstal";
    for (size_t pos = 1, prev_pos = 0; pos < str.size(); pos++) {
        if ((search.find(str[pos]) != string::npos && isdigit(str[prev_pos])) ||
            (str[prev_pos] == ')' && isdigit(str[pos])))
            str.insert(pos, "*");
        prev_pos = pos;
    }
}

void Model::parser(string str) {
    stack<string> operations;
    bool is_unary = true;
    for (size_t i = 0; i != str.size(); i++) {
        string str_by_pos = str.substr(i);
        if (isdigit(str[i])) {
            numbers_processing(i, str_by_pos, is_unary);
        } else if (str[i] == '-' && is_unary) {
            operations.push("~");
        } else if (str[i] == '+' && is_unary) {
            operations.push("plus");
        } else {
            operations_processing(operations, i, str_by_pos, is_unary);
        }
    }
    while (!operations.empty()) {
        this->polish_notation_.push(std::make_pair(operations.top(), 0));
        operations.pop();
    }
}

void Model::numbers_processing(size_t& index, string str, bool& is_unary) {
    this->polish_notation_.push(std::make_pair("num", stod(str)));
    index += shift_number(str) - 1;
    is_unary = false;
}

void Model::operations_processing(stack<string>& operations, size_t& index,
    string str, bool& is_unary) {
    string op = get_operations(str);
    if (op == " ") op = str[0];
    if (op == ")") {
        get_value_in_brackets(operations);
        is_unary = false;
    } else if (is_binary_operation(op) || is_unary_operation(op) || op == "(") {
        bool flag_prev_pow = true;
        if (op == "^" && operations.size() > 0)
            flag_prev_pow = !(operations.top() == "^");
        if (op != "(" && flag_prev_pow)
            prioritization(operations, op);
        operations.push(op);
        if (is_unary_operation(op))
            operations.push("(");
        is_unary = true;
        index += shift_operation(op);
    } else if (op != " ") {
        throw std::invalid_argument("Error: Wrong argument");
    }
}

string Model::get_operations(string str) {
    string op = " ";
    if (str.find("sin(") == 0)
        op = "sin";
    else if (str.find("cos(") == 0)
        op = "cos";
    else if (str.find("tan(") == 0)
        op = "tan";
    else if (str.find("acos(") == 0)
        op = "acos";
    else if (str.find("asin(") == 0)
        op = "asin";
    else if (str.find("atan(") == 0)
        op = "atan";
    else if (str.find("sqrt(") == 0)
        op = "sqrt";
    else if (str.find("ln(") == 0)
        op = "ln";
    else if (str.find("log(") == 0)
        op = "log";
    else if (str.find("mod") == 0)
        op = "mod";
    return op;
}

void Model::get_value_in_brackets(stack<string>& operations) {
    while (!operations.empty() && operations.top() != "(") {
        this->polish_notation_.push(std::make_pair(operations.top(), 0));
        operations.pop();
    }
    operations.pop();
}

void Model::prioritization(stack<string>& operations, string op) {
    while (!operations.empty() && priority(operations.top()) >= priority(op)) {
        this->polish_notation_.push(std::make_pair(operations.top(), 0));
        operations.pop();
    }
}

int Model::priority(string op) {
    int result = -1;
    if (op == "+" || op == "-" || op == "~" || op == "plus") {
        result = 1;
    } else if (op == "*" || op == "/" || op == "mod") {
        result = 2;
    } else if (op == "^") {
        result = 3;
    } else if (op == "sin" || op == "cos" || op == "tan" ||
            op == "asin" || op == "acos" || op == "atan" ||
            op == "log" || op == "ln" || op == "sqrt") {
        result = 4;
    }
    return result;
}

int Model::shift_operation(string op) {
    int result = 0;
    if (op == "ln" || op == "mod") {
        result = 2;
    } else if (op == "sin" || op == "cos" ||
            op == "log" || op == "tan") {
        result = 3;
    } else if (op == "sqrt" || op == "atan" ||
            op == "acos" || op == "asin") {
        result = 4;
    }
    return result;
}

int Model::shift_number(string value) {
    bool is_dot = false;
    int result = 0;
    while (isdigit(value[result]) || (value[result] == '.' && !is_dot)) {
        if (value[result] == '.') is_dot = true;
        result++;
    }
    return result;
}

void Model::calculate() {
    stack<double> intermediate_result;
    while (!polish_notation_.empty()) {
        string operation = this->polish_notation_.front().first;
        double value = this->polish_notation_.front().second;
        if (operation == "num") {
            intermediate_result.push(value);
        } else if (is_binary_operation(operation)) {
            perfoming_binary_operation(intermediate_result, operation);
        } else {
            perfoming_unary_operation(intermediate_result, operation);
        }
        this->polish_notation_.pop();
    }
    this->answer_ = intermediate_result.top();
}

void Model::perfoming_binary_operation(stack<double>& intermediate_result,
    string operation) {
    double result = 0;
    double value1 = get_value_from_stack(intermediate_result);
    double value2 = get_value_from_stack(intermediate_result);
    if (operation == "*") {
        result = value2 * value1;
    } else if (operation == "+") {
        result = value2 + value1;
    } else if (operation == "-") {
        result = value2 - value1;
    } else if (operation == "/") {
        check_division(value1);
        result = value2 / value1;
    } else if (operation == "mod") {
        result = fmod(value2, value1);
    } else if (operation == "^") {
        result = pow(value2, value1);
    }
    intermediate_result.push(result);
}

void Model::perfoming_unary_operation(stack<double>& intermediate_result,
    string operation) {
    double result = 0;
    double value = get_value_from_stack(intermediate_result);
    if (operation == "~") {
        result = -1 * value;
    } else if (operation == "plus") {
        result = value;
    } else if (operation == "sin") {
        result = sin(value);
    } else if (operation == "cos") {
        result = cos(value);
    } else if (operation == "tan") {
        result = tan(value);
    } else if (operation == "sqrt") {
        check_sqrt(value);
        result = sqrt(value);
    } else if (operation == "ln") {
        result = log(value);
    } else if (operation == "log") {
        result = log10(value);
    } else if (operation == "atan") {
        result = atan(value);
    } else if (operation == "acos") {
        result = acos(value);
    } else if (operation == "asin") {
        result = asin(value);
    }
    intermediate_result.push(result);
}

double Model::get_value_from_stack(stack<double>& intermediate_result) {
    if (intermediate_result.empty())
        throw std::invalid_argument("Not enought arguments");
    double value =  intermediate_result.top();
    intermediate_result.pop();
    return value;
}

bool Model::is_binary_operation(string op) {
    return (op == "*" || op == "-" || op == "+" || op == "mod" || op == "^" || op == "/");
}

bool Model::is_unary_operation(string op) {
    return (op == "sin" || op == "cos" || op == "tan" || op == "sqrt" || op == "ln" ||
            op == "log" || op == "atan" || op == "acos" || op == "asin" || op == "~" || op == "plus");
}

void Model::check_division(double value) {
    if (value == 0)
        throw std::invalid_argument("Error: Division by zero");
}

void Model::check_sqrt(double value) {
    if (value < 0)
        throw std::invalid_argument("Error: Sqrt of a negative number");
}

void Model::check_str(string str) {
    if (str.size() > 255)
        throw std::invalid_argument("Error: Too long expression");

    if (std::count(str.begin(), str.end(), '(') != std::count(str.begin(), str.end(), ')'))
        throw std::invalid_argument("Error: Wrong amount of brackets");

    size_t pos = 0;
    string temp = str.substr(pos);
    while ((pos = temp.find(")")) != string::npos) {
        size_t prev_pos = pos - 1;
        if (str[prev_pos] == '(')
            throw std::invalid_argument("Error: Empty argument in brackets");
        pos++;
        temp = temp.substr(pos);
    }
}

void Model::clear() {
    while (!this->polish_notation_.empty())
        this->polish_notation_.pop();
}

void Model::check_for_deposit(std::vector<pair<string, pair<int, double>>> list) {
    for (int i = 0; i < static_cast<int>(list.size()); i++) {
        for (int j = i - 1; j >= 0; j--) {
            if (list[j].first == list[i].first &&
                list[j].second.first == list[i].second.first)
                throw std::invalid_argument("Error: Two additions in one month");
        }
    }
}
