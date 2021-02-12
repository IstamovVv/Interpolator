#pragma once
#include "my_vector.h"
#include <iostream>
#include <memory>
#include <utility>

typedef std::shared_ptr<ist::vector<double>> double_vector_ptr;

double lagrange(const double_vector_ptr values_x, const double_vector_ptr values_y, double _x)
{
    float result = 0.0;

    for (size_t i = 0; i < values_x->size(); i++)
    {
        float P = 1.0;

        for (size_t j = 0; j < values_x->size(); j++)
            if (j != i)
                P *= (_x - (*values_x)[j]) / ((*values_x)[i] - (*values_x)[j]);

        result += P * (*values_y)[i];
    }

    return result;
}
