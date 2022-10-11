#ifndef SRC_VIEW_S21_VIEW_H
#define SRC_VIEW_S21_VIEW_H

#include "s21_view.h"
#include "ui_s21_view.h"

s21_view::s21_view(QWidget *parent) : QMainWindow(parent), ui(new Ui::s21_view) {
    ui->setupUi(this);

    ui->label_single_x->setVisible(0);
    Control = new s21::Controller();

    s21_view::validator();
    s21_view::keys_setting();

    credit_window = new credit_calc();
    connect(credit_window, &credit_calc::s21_view, this, &s21_view::show);

    depos_window = new depos_calc();
    connect(depos_window, &depos_calc::s21_view, this, &s21_view::show);
}

s21_view::~s21_view() {
    delete Control;
    delete ui;
}

void s21_view::check_radio() {
    if (ui->radioButton->isChecked() == 1)
        visibleView(1, 0);
    else
        visibleView(0, 1);
}

void s21_view::calculate() {
    putSizeX();
    std::pair<double, double> dataX = radio_res();
    if (ui->radioButton->isChecked() == 1) {
        makeGraph(dataX);
    } else {
        std::string stringForMove = (ui->text_box->text()).toStdString();
        std::pair<std::string, int> resultString = Control->calculate(stringForMove, dataX.second);
        ui->text_box->setText(QString::fromStdString(resultString.first));
    }
}

void s21_view::makeGraph(std::pair<double, double> dataX) {
    ui->widget->clearGraphs();
    ui->widget->xAxis->setRange(dataX.first, dataX.second);
    double xBegin = dataX.first, xEnd = dataX.second, resultY = 0, minY = DBL_MAX, maxY = DBL_MIN;
    QVector<double> x, y;
    int flag = 0;
    ui->widget->yAxis->setRange(DBL_MIN, DBL_MAX);

    for (double resultX = xBegin; resultX <= xEnd; resultX += setStep()) {
        std::pair<std::string, int> temp = Control->calculate(ui->text_box->text().toStdString(), resultX);
        if (temp.second == 0) {
            ui->text_box->setText(QString::fromStdString(temp.first));
            flag = 1;
            break;
        }

        resultY = std::stod(temp.first);
        putSizeY(resultY, &minY, &maxY);
        y.push_back(resultY);
        x.push_back(resultX);
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x, y);
        ui->widget->replot();
    }
    if (flag == 0)
        ui->text_box->setText("График готов");
    x.clear();
    y.clear();
}

std::pair<double, double> s21_view::radio_res() {
    std::pair<double, double> tmp_pair;
    if (ui->radioButton->isChecked() == 1) {
        tmp_pair.first = (ui->text_box_left->text()).toDouble();
        tmp_pair.second = (ui->text_box_right->text()).toDouble();
    } else {
        tmp_pair.first = (ui->text_box_right->text()).toDouble();
        tmp_pair.second = (ui->text_box_right->text()).toDouble();
    }
    return tmp_pair;
}

void s21_view::putSizeY(double resultY, double * minY, double * maxY) {
    if (resultY >= *maxY)
        *maxY = resultY;
    if (resultY <= *minY)
        *minY = resultY;
    ui->widget->yAxis->setRange(*minY, *maxY);
}

double s21_view::setStep() {
    return (fabs(ui->text_box_left->text().toDouble()) + fabs(ui->text_box_right->text().toDouble())) / 100;
}

void s21_view::putSizeX() {
    if (ui->text_box_left->text().toDouble() > ui->text_box_right->text().toDouble())
        ui->text_box_left->setText(ui->text_box_right->text());

    if (ui->text_box_left->text().toDouble() < -1000000)
        ui->text_box_left->setText("-1000000");
    if (ui->text_box_right->text().toDouble() > 1000000)
        ui->text_box_right->setText("1000000");

    if (ui->text_box_left->text().toDouble() > 1000000)
        ui->text_box_left->setText(ui->text_box_right->text());
    if (ui->text_box_right->text().toDouble() < -1000000)
        ui->text_box_right->setText(ui->text_box_left->text());
}

void s21_view::setRangeGraph(double x, double y) {
    ui->widget->yAxis->setRange(x, y);
}


void s21_view::visibleView(int x, int y) {
    ui->widget->setVisible(x);  // Отражение графика
    ui->text_box_left->setVisible(x);  // отражение мин Х
    ui->label->setVisible(x);  // отражение лейбла
    ui->label_2->setVisible(x);  // отражение лейбла
    ui->label_single_x->setVisible(y);  // отражение лейбла
}

void s21_view::trigonometry() {
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->text_box->text();
    QString tmp = button->text();
    tmp.resize(tmp.size() - 2);
    ui->text_box->setText(result_label + tmp);
}

void s21_view::backspace_delete() {
    QPushButton *button = (QPushButton *)sender();
    if (button->text() == "AC") {
        QString result_label = ui->text_box->text();
        result_label.resize(result_label.size() - 1);
        ui->text_box->setText(result_label);
    } else {
        ui->text_box->setText("");
    }
}

void s21_view::digits_numbers() {
    QPushButton *button = (QPushButton *)sender();
    QString result_label = ui->text_box->text();
    ui->text_box->setText(result_label + button->text());
}

void s21_view::financial() {
    QPushButton *button = (QPushButton *)sender();
    if (button->text() == "depos") {
        depos_window->show();
        this->close();
    } else {
        credit_window->show();
        this->close();
    }
}


void s21_view::validator() {
    ui->text_box_left->setValidator(new QDoubleValidator(this));
    ui->text_box_right->setValidator(new QDoubleValidator(this));
}

void s21_view::keys_setting() {
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(check_radio()));
    connect(ui->radioButton_2 , SIGNAL(clicked()), this, SLOT(check_radio()));

    connect(ui->Button_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_point, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_mul, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_sub, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_div, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_add, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_open_bracket, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_close_bracket, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_pow, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_mod, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->Button_x, SIGNAL(clicked()), this, SLOT(digits_numbers()));

    connect(ui->Button_backspace, SIGNAL(clicked()), this, SLOT(backspace_delete()));
    connect(ui->Button_delete, SIGNAL(clicked()), this, SLOT(backspace_delete()));

    connect(ui->Button_eq, SIGNAL(clicked()), this, SLOT(calculate()));

    connect(ui->Button_cos, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_sin, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_tan, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_acos, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_asin, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_atan, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_sqrt, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_ln, SIGNAL(clicked()), this, SLOT(trigonometry()));
    connect(ui->Button_log, SIGNAL(clicked()), this, SLOT(trigonometry()));

    connect(ui->Button_cred, SIGNAL(clicked()), this, SLOT(financial()));
    connect(ui->Button_depos, SIGNAL(clicked()), this, SLOT(financial()));
}

#endif  //  SRC_VIEW_S21_VIEW_H
