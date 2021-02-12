#include "graph.h"

#include <utility>
#include <cmath>

#include <QMouseEvent>

namespace GP
{
    int   WinWidth = 500,
          WinHeight = 500;

    int   WinFar = 10;
}

Graph::Graph(QWidget *parent): QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

Graph::~Graph()
{
}

void Graph::initializeGL()
{
    initializeOpenGLFunctions();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-GP::WinWidth/2, GP::WinWidth/2, -GP::WinHeight/2, GP::WinHeight/2, -GP::WinFar/2, GP::WinFar/2);
}

void Graph::paintGL()
{
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);
        glColor3f(1, 1, 1);

        // Горизонталь
        glVertex2f(-GP::WinWidth / 2, 0);
        glVertex2f(GP::WinWidth / 2, 0);

        // Вертикаль
        glVertex2f(0, GP::WinHeight);
        glVertex2f(0, -GP::WinHeight);

        float serif_h = delta/3; // Высота засечки

        // Засечки по горизонтали в положительном направлении
        for (float i = delta; i < GP::WinWidth / 2; i += delta)
        {
            glVertex2f(i, -serif_h);
            glVertex2f(i, serif_h);
        }

        // Засечки по горизонтали в отрицательном направлении
        for (float i = -delta; i > -GP::WinWidth / 2; i -= delta)
        {
            glVertex2f(i, -serif_h);
            glVertex2f(i, serif_h);
        }

        // Засечки по вертикали в положительном направлении
        for (float i = delta; i < GP::WinHeight / 2; i += delta)
        {
            glVertex2f(-serif_h, i);
            glVertex2f(serif_h, i);
        }

        // Засечки по вертикали в отрицательном направлении
        for (float i = -delta; i > -GP::WinHeight / 2; i -= delta)
        {
            glVertex2f(-serif_h, i);
            glVertex2f(serif_h, i);
        }
    glEnd();

    if (is_interpolated)
    {
        draw_function(i_values_x, i_values_y);
    } else
    {
        draw_function(values_x, values_y);
    }

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);

    glVertex2f(line_x, GP::WinHeight);
    glVertex2f(line_x, -GP::WinHeight);

    glEnd();

    glPointSize(5);
    glBegin(GL_POINTS);

    glColor3f(1, 1, 1);
    glVertex2f(line_x, point_y);

    glEnd();

}

void Graph::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void sort_values(double_vector_ptr values_x, double_vector_ptr values_y)
{
    double temp_x;
    double temp_y;

    for (size_t i = 0; i < values_x->size() - 1; i++) {
        for (size_t j = 0; j < values_x->size() - i - 1; j++) {
            if ((*values_x)[j] > (*values_x)[j + 1]) {
                // меняем элементы местами
                temp_x = (*values_x)[j];
                temp_y = (*values_y)[j];

                (*values_x)[j] = (*values_x)[j + 1];
                (*values_y)[j] = (*values_y)[j + 1];

                (*values_x)[j + 1] = temp_x;
                (*values_y)[j + 1] = temp_y;
            }
        }
    }
}

void Graph::get_input_data(double_vector_ptr input_values_x, double_vector_ptr input_values_y)
{
    if (values_x->size() != 0 || values_y->size() != 0)
    {
        for (const auto& value : *input_values_x)
                values_x->push_back(value);

            for (const auto& value : *input_values_y)
                values_y->push_back(value);
    } else
    {
        values_x = input_values_x;
        values_y = input_values_y;
    }

    sort_values(values_x, values_y);

    size_t index = 0;

    while (index < values_x->size() - 1)
    {
        if ((*values_x)[index] == (*values_x)[index + 1])
        {
            values_x->pull_out(index);
            values_y->pull_out(index);

        } else index++;
    }

    this->update();
}

float precision(float value)
{
    return round(value * 100) / 100;
}

float Graph::find_y_by_x(float x)
{
    x = precision(x);
    if (x < (*i_values_x)[0] || x > (*i_values_x)[i_values_x->size() - 1] || !is_interpolated) return line_y;

    int i = 0;
    while (precision((*i_values_x)[i]) > x) i++;
    while (precision((*i_values_x)[i]) < x) i++;

    return (*i_values_y)[i] * delta;
}

void Graph::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = mapFromGlobal(event->globalPos());
    int x = pos.x() - GP::WinWidth/2;
    int y = -pos.y() + GP::WinHeight/2;

    line_x = x;
    line_y = y;

    float transformed_x = (float)x/delta;

    point_y = find_y_by_x(transformed_x);

    emit send_coords(transformed_x, (float)point_y/delta);

    this->update();
}

void Graph::draw_function(double_vector_ptr values_x, double_vector_ptr values_y)
{
    if (!values_x->size()) return;

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);

    auto first_point = std::make_pair((*values_x)[0],(*values_y)[0]);

    for (size_t i = 1; i < values_x->size(); i++)
    {
        const auto& second_point = std::make_pair((*values_x)[i],(*values_y)[i]);

        glVertex2f(first_point.first * delta, first_point.second * delta);
        glVertex2f(second_point.first * delta, second_point.second * delta);

        first_point = second_point;
    }
    glEnd();
}
