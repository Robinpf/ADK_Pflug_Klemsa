#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>
#include <vector>

enum DrawMode {NONE, ANALYZE_POINT, POINTS, POLYGON};

class Draw : public QWidget
{
    Q_OBJECT
public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void saveActualPolygon();
    void drawPolygon(QPolygonF &polygon);
    void drawConvexHullPolygon(QPolygonF &polygon);
    void drawFilledPolygon(QPolygonF &polygon);
    void drawBorderPolygon(QPolygonF &polygon);
    void addFilledPolygon(QPolygonF &polygon){filledPolygons.push_back(polygon);}
    void addBorderPolygon(QPolygonF &polygon){borderPolygons.push_back(polygon);}
    void clearHighlitedPolygons(){filledPolygons.clear();borderPolygons.clear();}
    void clearConvexHullPolygon(){convexHull.clear();}
    void clearAll();

    // Getters & Setters
    void setDrawMode(DrawMode drawMode){this->drawMode = drawMode;}
    void setActualPolygon(QPolygonF &polygon){actualPolygon = polygon;}
    DrawMode getDrawMode(){return this->drawMode;}
    QVector<QPolygonF> getPolygons(){return polygons;}
    QPointF getAnalyzePoint(){return analyzePoint;}
    void setPolygons(QVector<QPolygonF> polygons);

    // Parameters
    QPointF analyzePoint; // Point to analyze with polygons.
    QVector<QPointF> points; // Single points.
    QVector<QPolygonF> polygons; // Vector of polygons.
    QVector<QPolygonF> filledPolygons; // Filled polygons.
    QVector<QPolygonF> borderPolygons; // Border polygons.
    QPolygonF actualPolygon; // Vector of actual drawing polygon.
    QPolygonF convexHull; // Polygon witch represented convex hull.

private:
    DrawMode drawMode;
signals:

public slots:
};



#endif // DRAW_H
