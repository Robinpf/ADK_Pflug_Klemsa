#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPoint>
#include <vector>
#include <QPolygonF>

class Algorithms
{
private:
	std::vector<QPoint> polygon;
	QPoint q;
public:

    Algorithms();
    static int getPointLinePosition(QPointF &q,QPointF &p1,QPointF &p2);
    static double getAngle2Vectors(QPointF &p1,QPointF &p2,QPointF &p3,QPointF &p4);
    static int positionPointPolygonWinding(QPointF &q, QPolygonF &pol);
    static int positionPointPolygonRayCrossing(QPointF &q, QPolygonF &pol);
    static QPolygonF createRandomPolygon();
};

#endif // ALGORITHMS_H
