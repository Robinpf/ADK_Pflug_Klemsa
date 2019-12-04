#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"

enum AnalyzeType {SLOPE, ASPECT, HYPSOMETRY};

class Draw : public QWidget
{

    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;
    std::vector<Edge> contours;
    std::vector<Triangle> dtm;


public:
    explicit Draw(QWidget *parent = nullptr);
    void setPoints(std::vector<QPoint3D> &points_){points=points_;}
    std::vector<Edge>& getDt() {return dt;}
    void setDt(std::vector<Edge> &dt_){dt=dt_;}
    void setContours(std::vector<Edge> &contours_){contours=contours_;}
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    std::vector<QPoint3D> getPoints(){return points;}
    void clearPoints(){points.clear(); }
    void clearDT(){dt.clear();}
    int getDtSize(){return dt.size();}
    void setDTM(std::vector<Triangle> & dtm_){dtm = dtm_;}
    static void importPoints(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &z_min, double &z_max);

    AnalyzeType analyzeType = AnalyzeType::SLOPE;
signals:

public slots:
};

#endif // DRAW_H
