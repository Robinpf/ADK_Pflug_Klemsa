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
    void on_btn_imp_polygons_clicked();

    void on_btn_drw_none_clicked();

    void on_btn_drw_point_clicked();

    void on_btn_drw_polygon_clicked();

    void on_btn_anl_polygon_point_clicked();

    void on_btn_gen_rand_polygon_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
