#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_1_0>
#include <QSize>

#include "my_vector.h"

typedef std::shared_ptr<ist::vector<double>> double_vector_ptr;

class Graph : public QOpenGLWidget, public QOpenGLFunctions_1_0
{
    Q_OBJECT;
public:
    Graph(QWidget *parent = 0);
    ~Graph();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

friend class Interpolator;
//friend class Input_widget;

private slots:
    void get_input_data(double_vector_ptr input_values_x, double_vector_ptr input_values_y);

signals:
    void send_coords(float, float);

private:
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    float find_y_by_x(float x);

private:
    void draw_function(double_vector_ptr values_x, double_vector_ptr values_y);

private:
    double_vector_ptr values_x = std::make_shared<ist::vector<double>>();
    double_vector_ptr values_y = std::make_shared<ist::vector<double>>();

    double_vector_ptr i_values_x = std::make_shared<ist::vector<double>>();
    double_vector_ptr i_values_y = std::make_shared<ist::vector<double>>();

private:
    bool is_interpolated = false;

private:
    int delta = 5;

    int line_x = 0;
    int line_y = 0;
    int point_y = 0;

};

#endif // MYWIDGET_H
