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
    case DrawMode::POINTSD:
        // Draw single points.
        points.push_back(p);
        break;
    case DrawMode::ANALYZE_POINT:
        // Draw analyze point.
        analyzePoint = p;
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

    // Draw analyze point
    int ap_r = 10; // Analize point radius.
    int sp_r = 5; // Single point radius.
    double pp_r = 1.3;
    //Draw point Q
    qp.drawEllipse(analyzePoint.x() - ap_r/2,analyzePoint.y() - ap_r/2, ap_r, ap_r);
    qp.drawLine(analyzePoint.x()-pp_r*ap_r, analyzePoint.y(), analyzePoint.x()+pp_r*ap_r, analyzePoint.y());
    qp.drawLine(analyzePoint.x(), analyzePoint.y()-pp_r*ap_r, analyzePoint.x(), analyzePoint.y()+pp_r*ap_r);

    // Draw all points
    for (int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x() - sp_r/2,points[i].y() - sp_r/2, sp_r, sp_r);
    }

    // Draw actual polygon.
    drawPolygon(actualPolygon);

    // Draw all poly
    for (int i = 0; i < polygons.size(); i++)
    {
        drawPolygon(polygons[i]);
    }

    // Draw all poly filled.
    for (int i = 0; i < filledPolygons.size(); i++)
    {
        drawFilledPolygon(filledPolygons[i]);
    }

    // Draw highlited polygons.
    for (int i = 0; i < borderPolygons.size(); i++)
    {
        drawBorderPolygon(borderPolygons[i]);
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
