#include "ui_interpolator.h"
#include "interpolator.h"

#include "input_widget.h"
#include "lagrange.h"

#include <cmath>

Interpolator::Interpolator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Interpolator)
{
    ui->setupUi(this);
    this->setFixedWidth(850);
    this->setFixedHeight(520);

    QObject::connect(ui->GLWidget, SIGNAL(send_coords(float, float)),
                     this,         SLOT(get_coords(float, float)));
}

Interpolator::~Interpolator()
{
    delete ui;
    delete window;
}

void Interpolator::get_coords(float x, float y)
{
    QString str = "X: ";
    str += QString::number(x) + '\n';
    str += "Y: ";
    str += QString::number(y) + '\n';

    ui->console_box->setText(str);
}

void Interpolator::input_widget_closed()
{
    is_input_widget_closed = true;
}

void Interpolator::on_input_button_clicked()
{
    ui->GLWidget->is_interpolated = false;

    ui->GLWidget->values_x->clear();
    ui->GLWidget->values_y->clear();

    ui->GLWidget->i_values_x->clear();
    ui->GLWidget->i_values_y->clear();

    if (is_input_widget_closed)
    {
        window = new Input_widget;

        QObject::connect(window, SIGNAL(send_data(double_vector_ptr, double_vector_ptr)),
                         ui->GLWidget, SLOT(get_input_data(double_vector_ptr, double_vector_ptr)));

        QObject::connect(window, SIGNAL(closed()),
                         this,   SLOT(input_widget_closed()));

        window->show();

        is_input_widget_closed = false;
    }
}

void Interpolator::on_add_button_clicked()
{
    ui->GLWidget->is_interpolated = false;

    ui->GLWidget->i_values_x->clear();
    ui->GLWidget->i_values_y->clear();

    if (is_input_widget_closed)
    {
        window = new Input_widget;

        QObject::connect(window, SIGNAL(send_data(double_vector_ptr, double_vector_ptr)),
                         ui->GLWidget, SLOT(get_input_data(double_vector_ptr, double_vector_ptr)));

        QObject::connect(window, SIGNAL(closed()),
                         this,   SLOT(input_widget_closed()));

        window->show();

        is_input_widget_closed = false;
    }

}

void Interpolator::on_interpolate_button_clicked()
{
    double delta = 0.01;

    double_vector_ptr values_x = ui->GLWidget->values_x;
    double_vector_ptr values_y = ui->GLWidget->values_y;

    double_vector_ptr i_values_x = ui->GLWidget->i_values_x;
    double_vector_ptr i_values_y = ui->GLWidget->i_values_y;

    ui->GLWidget->i_values_x->clear();
    ui->GLWidget->i_values_y->clear();

    double start = (*values_x)[0];
    double end = (*values_x)[values_x->size() - 1];

    for (double x = start; x <= end; x += delta)
    {
        double y = lagrange(values_x, values_y, x);
        i_values_x->push_back(round(x * 100) / 100);
        i_values_y->push_back(round(y * 100) / 100);
    }

    ui->GLWidget->is_interpolated = true;

    ui->GLWidget->update();
}

void Interpolator::on_get_original_button_clicked()
{
    ui->GLWidget->is_interpolated = false;

    ui->GLWidget->update();
}


void Interpolator::on_clear_button_clicked()
{
    ui->GLWidget->is_interpolated = false;

    ui->GLWidget->values_x->clear();
    ui->GLWidget->values_y->clear();

    ui->GLWidget->i_values_x->clear();
    ui->GLWidget->i_values_y->clear();

    ui->GLWidget->update();
}

void Interpolator::on_scale_slider_sliderMoved(int position)
{
    ui->GLWidget->delta = position + 5;
    ui->GLWidget->update();
}
