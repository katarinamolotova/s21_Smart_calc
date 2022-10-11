#include "../model/s21_model.h"
#include "../controller/s21_controller.h"
// #include "../view/s21_view.h"
#include <gtest/gtest.h>

class S21CalcTest : public testing::Test {
 protected:
    s21::Controller *Controll = new s21::Controller();

    void SetUp() {
        // std::pair <double, double> dataX;
        // dataX.first = 10;
        // dataX.second = 10;
        // std::string test_1 = "2^2^3";
    }

    void TearDown() {
        delete Controll;
    }
};

/*
    (макрос TEST_F() используется вместо TEST(), так как тестирующая 
    функция должна иметь доступ к полям и методам класса)
*/ 
TEST_F(S21CalcTest, megaTest_1) {
    std::string megaTest_1 = "2^2^3";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(megaTest_1, dataX.second);

    EXPECT_EQ("256.000000", resultString.first);
}

TEST_F(S21CalcTest, megaTest_2) {
    std::string megaTest_2 = "14^23/6*9+5-1+(56*2)";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(megaTest_2, dataX.second);

    EXPECT_EQ("344378539330472184817647616.000000", resultString.first);
}

TEST_F(S21CalcTest, megaTest_3) {
    std::string megaTest_3 = "cos(45)+sin(45)+tan(45)+acos(45)+asin(45)+atan(45)+sqrt(45)+ln(45)+log(45)";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(megaTest_3, dataX.second);

    EXPECT_EQ("nan", resultString.first);
}

TEST_F(S21CalcTest, Negatove) {
    std::string Negatove = "sqrt(-1)";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(Negatove, dataX.second);

    EXPECT_EQ("Error: Sqrt of a negatove number", resultString.first);
}

TEST_F(S21CalcTest, DivisionByZero) {
    std::string DivisionByZero = "3/0";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(DivisionByZero, dataX.second);

    EXPECT_EQ("Error: Division by zero", resultString.first);
}

TEST_F(S21CalcTest, TestX) {
    std::string TestX = "x+3*10";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(TestX, dataX.second);

    EXPECT_EQ("40.000000", resultString.first);
}

TEST_F(S21CalcTest, WrongArgument) {
    std::string WrongArgument = "fghjk";
    std::pair <double, double> dataX;
    dataX.first = 10;
    dataX.second = 10;
    std::pair<std::string, int> resultString = Controll->calculate(WrongArgument, dataX.second);

    EXPECT_EQ("Error: Wrong argument", resultString.first);
}

TEST_F(S21CalcTest, Credit_1) {
    std::string Sum = "10000.000";
    std::pair<std::string, int> tmpData;
    std::string Percent = "10";
    tmpData.first = "3";
    tmpData.second = 0;

    std::vector<double> result = Controll->every_moth_pay(1, Sum, tmpData, Percent);
    std::vector<double> atalon = {3389.04, 3389.04, 3389.04};

    int period = stoi(tmpData.first);
    for (int i = 0; i < period; i++) {
         EXPECT_EQ(round(atalon[i]*100)/100, round(result[i]*100)/100);
    }
}

TEST_F(S21CalcTest, Credit_2) {
    std::string Sum = "10000.000";
    std::pair<std::string, int> tmpData;
    std::string Percent = "10";
    tmpData.first = "3";
    tmpData.second = 0;

    std::vector<double> result = Controll->every_moth_pay(0, Sum, tmpData, Percent);
    std::vector<double> atalon = {3416.67, 3388.19, 3359.96};

    int period = stoi(tmpData.first);
    for (int i = 0; i < period; i++) {
         EXPECT_EQ(round(atalon[i]*100)/100, round(result[i]*100)/100);
    }
}

TEST_F(S21CalcTest, Credit_Total) {
    std::string Sum = "10000.000";
    std::pair<std::string, int> tmpData;
    std::string Percent = "10";
    tmpData.first = "3";
    tmpData.second = 0;

    std::vector<double> result = Controll->every_moth_pay(0, Sum, tmpData, Percent);
    double res = Controll->total_payment(result);
    EXPECT_EQ(10164.82, round(res * 100)/100);
}

TEST_F(S21CalcTest, Credit_percent) {
    std::string Sum = "10000.000";
    std::pair<std::string, int> tmpData;
    std::string Percent = "10";
    tmpData.first = "3";
    tmpData.second = 0;

    std::vector<double> result = Controll->every_moth_pay(0, Sum, tmpData, Percent);
    double res = Controll->total_payment(result);
    double total_percent = Controll->overpay(res, Sum);
    EXPECT_EQ(164.82, round(total_percent * 100)/100);
}

// double Controller::overpay(double total_payment, string sum)


TEST_F(S21CalcTest, Deposit) {
    string sum = "10000", procent = "10", month_start = "6";
    std::pair <string, int> amount_of_month;
    amount_of_month.first = "3";
    amount_of_month.second = 0;
    int capitaliz = 1, period_pay = 0;
    std::vector<pair<string, pair<int, double>>> additions;
    std::vector<pair<string, pair<int, double>>> withdrawal;
    
    double result = Controll->result_procent(sum, amount_of_month, procent, capitaliz, period_pay,
        month_start, additions, withdrawal);

    EXPECT_EQ(254.18, round(result *100)/100);
}

TEST_F(S21CalcTest, Deposit_total) {
    string sum = "10000", procent = "10", month_start = "6";
    std::pair <string, int> amount_of_month;
    amount_of_month.first = "3";
    amount_of_month.second = 0;
    int capitaliz = 1, period_pay = 0;
    std::vector<pair<string, pair<int, double>>> additions;
    std::vector<pair<string, pair<int, double>>> withdrawal;
    
    double result = Controll->result_procent(sum, amount_of_month, procent, capitaliz, period_pay,
        month_start, additions, withdrawal);
    double total_res = Controll->sum_at_the_end(sum, result, additions, withdrawal);


    EXPECT_EQ(10254.18, round(total_res *100)/100);
}

TEST_F(S21CalcTest, Deposit_nalog) {
    string sum = "10000", procent = "10", month_start = "6";
    std::pair <string, int> amount_of_month;
    amount_of_month.first = "3";
    amount_of_month.second = 0;
    int capitaliz = 1, period_pay = 0;
    std::vector<pair<string, pair<int, double>>> additions;
    std::vector<pair<string, pair<int, double>>> withdrawal;
    
    double result = Controll->result_procent(sum, amount_of_month, procent, capitaliz, period_pay,
        month_start, additions, withdrawal);
    double total_res = Controll->sum_nalog("1", result);

    EXPECT_EQ(2.54, round(total_res *100)/100);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
