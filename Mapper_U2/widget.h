#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "vector"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private slots:
    //void on_btn_drw_none_clicked();

    void on_btn_clear_clicked();

    void on_btn_drw_points_clicked();

    void on_btn_gen_convex_hull_clicked();

    void on_btn_gen_rand_points_clicked();

    void on_btn_tst_algorithms_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
