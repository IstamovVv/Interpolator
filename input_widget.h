#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <memory>
#include "my_vector.h"

typedef std::shared_ptr<ist::vector<double>> double_vector_ptr;

namespace Ui {
class Input_widget;
}

class Input_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Input_widget(QWidget *parent = nullptr);
    ~Input_widget();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void closed();

signals:
    void send_data(double_vector_ptr, double_vector_ptr);

private:
    Ui::Input_widget *ui;
};

#endif // INPUT_WIDGET_H
