#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPoint>
#include <vector>

class Algorithms
{
private:
	std::vector<QPoint> polygon;
	QPoint q;
public:

    Algorithms();
    static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
    static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
    static int positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol);
    static int positionPointPolygonRayCrossing(QPoint &q, std::vector<QPoint> &pol);
    static std::vector<QPoint> createRandomPolygon();
};

#endif // ALGORITHMS_H
