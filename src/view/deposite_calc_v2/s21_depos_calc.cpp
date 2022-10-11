#include "s21_depos_calc.h"
#include "ui_depos_calc.h"
#include <QMessageBox>


depos_calc::depos_calc(QWidget *parent) : QDialog(parent), ui(new Ui::depos_calc) {
    ui->setupUi(this);
    control = new s21::Controller();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(back()));
    validatorSetting();
}

depos_calc::~depos_calc() {
    delete control;
    delete ui;
}

void depos_calc::back() {
    this->close();
    emit s21_view();
}

void depos_calc::validatorSetting() {
    ui->lineEdit->setValidator(new QDoubleValidator(this));
    ui->lineEdit_2->setValidator(new QIntValidator(1, 600,this));
    ui->lineEdit_3->setValidator(new QDoubleValidator(this));
    ui->lineEdit_4->setValidator(new QDoubleValidator(this));
    ui->add->setValidator(new QDoubleValidator(this));
    ui->sub->setValidator(new QDoubleValidator(this));
}



void depos_calc::on_run_clicked() {
    std::string sum =  ui->lineEdit->text().toStdString();
    std::pair<std::string, int> amount_of_month =
            std::make_pair(ui->lineEdit_2->text().toStdString(), ui->month_or_year->currentIndex());
    std::string procent = ui->lineEdit_3->text().toStdString();
    std::string nalog_procent = ui->lineEdit_4->text().toStdString();
    std::string start_data = ui->data->date().toString("MM").toStdString();

    int period_pay = set_value_for_capitalis_and_period(ui->period->currentIndex());
    int capitalis = set_value_for_capitalis_and_period(ui->capitalis->currentIndex());

    if (control->check_list_add_and_sub(additions) && control->check_list_add_and_sub(withdrawal)) {
        double result_procent = control->result_procent(sum, amount_of_month, procent, capitalis, period_pay, start_data,
                                                             additions, withdrawal);
        double nalog_sum = control->sum_nalog(nalog_procent, result_procent);
        double mutual_sum = control->sum_at_the_end(sum, result_procent, additions, withdrawal);
        ui->result_procent->setText(QString::number(result_procent, 'f', 2));
        ui->nalog_sum->setText(QString::number(nalog_sum, 'f', 2));
        ui->mutual_sum->setText(QString::number(mutual_sum, 'f', 2));
    } else {
        QMessageBox::warning(this, "Error", "Можно снять/добавить деньги на счет только раз в месяц");
    }
}

int depos_calc::set_value_for_capitalis_and_period(int value) {
    return (value == 2) ? 3 : (value == 3) ? 12 : value;
}

void depos_calc::on_add_add_clicked() {
    additions.push_back(std::make_pair(ui->add_data->date().toString("yy").toStdString(),
        std::make_pair(ui->add_data->date().toString("MM").toInt(),
        ui->add->text().toDouble())));
    ui->add_list->insertPlainText(ui->add_data->date().toString("dd/MM/yyyy") + " " + ui->add->text() + " руб.\n");
}

void del(QTextEdit *list) {
    QTextCursor cursor = list->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    cursor.deletePreviousChar();
    list->setTextCursor(cursor);
}

void depos_calc::on_del_add_clicked() {
    if (!additions.empty())
        additions.pop_back();
    del(ui->add_list);
    del(ui->add_list);
    if (!(ui->add_list->document()->isEmpty()))
        ui->add_list->insertPlainText("\n");
}


void depos_calc::on_sub_add_clicked() {
    withdrawal.push_back(std::make_pair(ui->sub_data->date().toString("yy").toStdString(),
        std::make_pair(ui->sub_data->date().toString("MM").toInt(),
        ui->sub->text().toDouble())));
    ui->sub_list->insertPlainText(ui->sub_data->date().toString("dd/MM/yyyy") + " " + ui->sub->text() + " руб.\n");
}

void depos_calc::on_del_sub_clicked() {
    if (!withdrawal.empty())
        withdrawal.pop_back();
    del(ui->sub_list);
    del(ui->sub_list);
    if (!(ui->sub_list->document()->isEmpty()))
        ui->sub_list->insertPlainText("\n");
}
