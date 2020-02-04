#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    drawMode = DrawMode::NONE;
    analyzePoint.setX(-99999);
    analyzePoint.setY(-99999);
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    QPoint p;
    p.setX(e->x());
    p.setY(e->y());
    switch (drawMode) {
    case DrawMode::NONE:
        // Do nothing.
        break;
    case DrawMode::P0OINTS:
        // Draw single points.
        points.push_back(p);
        break;
    case DrawMode::POLYGON:
        // Draw polygon.
        actualPolygon.push_back(p);

        break;
    default:
        break;
    }

    // Repaint canvas.
    repaint();
}

void Draw::paintEvent(QPaintEvent *e)

{
    QPainter qp(this);
    qp.begin(this);
    double sp_r = 5.0;

    // Draw all points
    for (int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x() - sp_r/2,points[i].y() - sp_r/2, sp_r, sp_r);        
    }

    // Draw convex hull.
    drawPolygon(convexHull);

    qp.end();
}

void Draw:: clearAll()
{
    this->clearHighlitedPolygons();
    polygons.clear();
    analyzePoint.setX(-999);analyzePoint.setY(-999);
    points.clear();
    convexHull.clear();
    repaint();
}

void Draw::drawPolygon(QPolygonF &polygon)
{
    QPainter qp(this);
    qp.begin(this);
    QColor col;
    col = QColor(255,0,0);
    qp.setPen(col);

    // Draw actual polygon.
    int apoly_r = 5;
    for (int i = 0; i < polygon.size(); i++)
    {
        qp.drawEllipse(polygon[i].x() - apoly_r/2,polygon[i].y() - apoly_r/2, apoly_r, apoly_r);
    }

    qp.drawPolygon(polygon);

    qp.end();
}

void Draw::drawConvexHullPolygon(QPolygonF &polygon)
{
    QPainter qp(this);
    qp.begin(this);

    // Draw actual polygon.
    int apoly_r = 5;
    for (int i = 0; i < polygon.size(); i++)
    {
        qp.drawEllipse(polygon[i].x() - apoly_r/2,polygon[i].y() - apoly_r/2, apoly_r, apoly_r);
    }

    qp.drawPolygon(polygon);

    qp.end();
}

void Draw::drawFilledPolygon(QPolygonF &polygon)
{
    // Start QPainter.
    QPainter qp(this);
    qp.begin(this);

    // Set brush.
    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::Dense3Pattern);
    QPainterPath painterPath;

    painterPath.addPolygon(polygon);
    qp.fillPath(painterPath, brush);
    qp.drawPolygon(polygon);

    qp.end();
}

void Draw::drawBorderPolygon(QPolygonF &polygon)
{
    // Start QPainter.
    QPainter qp(this);
    qp.begin(this);

    // Set brush.
    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::Dense3Pattern);
    QPainterPath painterPath;

    painterPath.addPolygon(polygon);
    qp.fillPath(painterPath, brush);
    qp.drawPolygon(polygon);

    qp.end();
}

void Draw::saveActualPolygon()
{
    if (!actualPolygon.isEmpty()) {
        polygons.push_back(actualPolygon);
        actualPolygon.clear();
    }
}

void Draw::setPolygons(QVector<QPolygonF> polygons)
{
    this->polygons = polygons;
    repaint();
}
