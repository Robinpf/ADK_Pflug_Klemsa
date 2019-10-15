#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>
#include <vector>

enum DrawMode {NONE, ANALYZE_POINT, POLYGON};

class Draw : public QWidget
{
    Q_OBJECT
public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void saveActualPolygon();
    void drawPolygon(std::vector<QPoint>);
    void drawFilledPolygon(std::vector<QPoint>);


    // Getters & Setters
    void setDrawMode(DrawMode drawMode){this->drawMode = drawMode;}
    void setActualPolygon(std::vector<QPoint> polygon){actualPolygon = polygon;}
    DrawMode getDrawMode(){return this->drawMode;}
    std::vector<std::vector<QPoint>> getPolygons(){return polygons;}
    QPoint getAnalyzePoint(){return analyzePoint;}
    void setPolygons(std::vector<std::vector<QPoint>> polygons);

    // Parameters
    QPoint analyzePoint;
    std::vector<std::vector<QPoint>> polygons; // vector of polygons.
    std::vector<QPoint> actualPolygon; // vector of actual drawing polygon.

private:
    DrawMode drawMode;
signals:

public slots:
};



#endif // DRAW_H
