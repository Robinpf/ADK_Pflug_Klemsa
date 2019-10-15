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
    int ap_r = 10;
    double ap_p = 1.3;
    //Draw point Q
    qp.drawEllipse(analyzePoint.x() - ap_r/2,analyzePoint.y() - ap_r/2, ap_r, ap_r);
    qp.drawLine(analyzePoint.x()-ap_p*ap_r, analyzePoint.y(), analyzePoint.x()+ap_p*ap_r, analyzePoint.y());
    qp.drawLine(analyzePoint.x(), analyzePoint.y()-ap_p*ap_r, analyzePoint.x(), analyzePoint.y()+ap_p*ap_r);

    // Draw actual polygon.
    drawPolygon(actualPolygon);

    // Draw all poly
    for (int i = 0; i < polygons.size(); i++)
    {
        drawPolygon(polygons[i]);
    }

    qp.end();
}

void Draw::drawPolygon(std::vector<QPoint> polygon)
{
    QPainter qp(this);
    qp.begin(this);

    // Draw actual polygon.
    int apoly_r = 5;
    for (int i = 0; i < polygon.size(); i++)
    {
        qp.drawEllipse(polygon[i].x() - apoly_r/2,polygon[i].y() - apoly_r/2, apoly_r, apoly_r);
    }

    QPolygon qpoly;
    for(int i = 0;i < polygon.size(); i++)
    {
        qpoly.append(polygon[i]);
    }

    qp.drawPolygon(qpoly);

    qp.end();
}

void Draw::drawFilledPolygon(std::vector<QPoint> polygon)
{
    QPainter qp(this);
    qp.begin(this);

    qp.end();
}

void Draw::saveActualPolygon()
{
    polygons.push_back(actualPolygon);
    actualPolygon.clear();
}

void Draw::setPolygons(std::vector<std::vector<QPoint>> polygons)
{
    this->polygons = polygons;
    repaint();
}
