#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "vector"
#include "geometrical.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    // Add new geometrical to geometrical vector.
    void addGeometrical(Geometrical geometrical){geomObjects.push_back(geometrical);}

private slots:
    void on_btn_imp_polygons_clicked();

    void on_btn_drw_none_clicked();

    void on_btn_drw_point_clicked();

    void on_btn_drw_polygon_clicked();

    void on_btn_anl_polygon_point_clicked();

private:
    Ui::Widget *ui;

    std::vector<Geometrical> geomObjects;
};

#endif // WIDGET_H
