#include "s21_credit_calc.h"
#include "ui_s21_credit_calc.h"

credit_calc::credit_calc(QWidget *parent) : QDialog(parent), ui(new Ui::credit_calc) {
    ui->setupUi(this);
    Control = new s21::Controller();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(back()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(calculate()));
    validatorSetting();
}

credit_calc::~credit_calc() {
    delete Control;
    delete ui;
}

void credit_calc::back() {
    this->close();
    emit s21_view();
}

void credit_calc::calculate() {
    ui->textEdit->clear();
    checkData();

    std::string Sum = ui->lineEdit->text().toStdString();
    std::pair<std::string, int> tmpData;
    std::string Percent = ui->lineEdit_3->text().toStdString();
    tmpData.first = ui->lineEdit_2->text().toStdString();
    tmpData.second = ui->comboBox->currentIndex();

    std::vector<double> result = Control->every_moth_pay(ui->radioButton->isChecked(), Sum, tmpData, Percent);
    print(result, Sum);
}

void credit_calc::print(std::vector<double> result, std::string Sum) {
    int period = ui->lineEdit_2->text().toInt();
    if ( ui->comboBox->currentIndex() == 1)
        period *= 12;

    for (int i = 1, j = 0; i <= period; i++, j++) {
        ui->textEdit->insertPlainText("Месяц " + QString::number(i) + ": " + QString::number(result[j], 'f', 2) + "\n");
        ui->textEdit->textCursor().movePosition(QTextCursor::Start);
    }
    ui->lineEdit_4->setText(QString::number(Control->total_payment(result), 'f', 2));
    ui->lineEdit_6->setText(QString::number(Control->overpay(Control->total_payment(result), Sum), 'f', 2));
}

void credit_calc::validatorSetting() {
    ui->lineEdit->setValidator(new QDoubleValidator(this));
    ui->lineEdit_2->setValidator(new QIntValidator(1, 600,this));
    ui->lineEdit_3->setValidator(new QDoubleValidator(this));
}

void credit_calc::checkData() {
    checkSum();
    checkPeriod();
    checkProcent();
}

void credit_calc::checkSum() {
    double tmpSum = ui->lineEdit->text().toDouble();
    if(tmpSum < 0)
        tmpSum = std::fabs(tmpSum);
    if(tmpSum > 50000000)
        tmpSum = 50000000;
    ui->lineEdit->setText(QString::number(tmpSum, 'f', 3));
}

void credit_calc::checkPeriod() {
    int tmpPeriod = ui->lineEdit_2->text().toInt();
    if (tmpPeriod <= 0)
        (tmpPeriod < 0) ? tmpPeriod = std::abs(tmpPeriod) : tmpPeriod = 1;
    if (ui->comboBox->currentIndex() == 0) {
        (tmpPeriod > 600) ? tmpPeriod = 600 : tmpPeriod;
    } else {
        (tmpPeriod > 50) ? tmpPeriod = 50 : tmpPeriod;
    }
    ui->lineEdit_2->setText(QString::number(tmpPeriod));
}

void credit_calc::checkProcent() {
    double tmpProcent = ui->lineEdit_3->text().toDouble();
    if(tmpProcent <= 0)
        (tmpProcent == 0) ? tmpProcent = std::fabs(tmpProcent) : tmpProcent = std::fabs(tmpProcent) ;
    if(tmpProcent > 100)
        tmpProcent = 100;
    ui->lineEdit_3->setText(QString::number(tmpProcent, 'f', 2));
}
