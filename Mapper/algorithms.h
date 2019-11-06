#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPoint>
#include <vector>
#include <QPolygonF>
#include <QtGui>

enum Shape {CIRCLE, SQUARE, GRID, RASTER};

class Algorithms
{
private:
	std::vector<QPoint> polygon;
	QPoint q;
public:

    Algorithms();
    static int getPointLinePosition(QPointF &q,QPointF &p1,QPointF &p2);
    static double getPointLineDistance(QPointF &q, QPointF &p1, QPointF &p2);
    static double getAngle2Vectors(QPointF &p1,QPointF &p2,QPointF &p3,QPointF &p4);
    static int positionPointPolygonWinding(QPointF &q, QPolygonF &pol);
    static int positionPointPolygonRayCrossing(QPointF &q, QPolygonF &pol);
    static QPolygonF createRandomPolygon();
    static QVector<QPointF> generateRandomPoints(int numberOfPoints, Shape shape);

    static QPolygonF strictlyConvex(QPolygonF &ch);
    static QPolygonF jarvisScan(QVector<QPointF> &points);
    static QPolygonF qHull(QVector<QPointF> &points);
    static void qh(int s, int e, QVector<QPointF> &points, QPolygonF &ch);
    static QPolygonF sweepLine(QVector<QPointF> &points);
    static QPolygonF grahamScan(QVector<QPointF> &points);
};

#endif // ALGORITHMS_H
