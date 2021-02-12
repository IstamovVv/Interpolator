#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <QMainWindow>
#include "input_widget.h"

typedef std::shared_ptr<ist::vector<double>> double_vector_ptr;

namespace Ui {
class Interpolator;
}

class Interpolator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Interpolator(QWidget *parent = nullptr);
    ~Interpolator();

private slots:
    void on_input_button_clicked();

    void on_add_button_clicked();

    void on_interpolate_button_clicked();

    void on_get_original_button_clicked();

    void on_clear_button_clicked();

    void on_scale_slider_sliderMoved(int position);

    void get_coords(float x, float y);

    void input_widget_closed();

private:
    bool is_input_widget_closed = true;

private:
    Ui::Interpolator *ui;
    Input_widget *window;
};

#endif // INTERPOLATOR_H
