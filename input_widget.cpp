#include "ui_input_widget.h"
#include "input_widget.h"

#include <QString>
#include <iostream>
#include <string>
#include <regex>

double_vector_ptr parse(const std::string& str)
{
    double_vector_ptr values = std::make_shared<ist::vector<double>>();

    std::regex words_regex("(^|\\s)-?\\d{1,4}(\\.\\d{1,7})?");

    auto words_begin = std::sregex_iterator(str.begin(), str.end(), words_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();
        double value = std::stod(match_str);
        values->push_back(value);
    }

    return values;
}

Input_widget::Input_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Input_widget)
{
    ui->setupUi(this);
}

Input_widget::~Input_widget()
{
    delete ui;
}

typedef std::shared_ptr<ist::vector<double>> double_vector_ptr;

void Input_widget::on_buttonBox_accepted()
{
    double_vector_ptr values_x = std::make_shared<ist::vector<double>>();
    double_vector_ptr values_y = std::make_shared<ist::vector<double>>();

    QString input_x = ui->input_x->text();
    QString input_y = ui->input_y->text();

    if (input_x.size() == 0 || input_y.size() == 0)
    {
        this->close();
        return;
    }

    std::string str_x = input_x.toStdString();
    std::string str_y = input_y.toStdString();

    values_x = parse(str_x);
    values_y = parse(str_y);

    while (values_x->size() > values_y->size())
         values_x->pop_back();

    while (values_y->size() > values_x->size())
         values_y->pop_back();

    if (values_x->size() > 0)
    {
        emit send_data(values_x, values_y);
    }

    emit closed();

    this->close();
}

void Input_widget::on_buttonBox_rejected()
{
    emit closed();
}
