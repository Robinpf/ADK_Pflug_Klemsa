#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
#include <list>
#include "qpoint3d.h"
#include "edge.h"
#include "sortbyx.h"
#include "triangle.h"

class Algorithms
{
public:
    Algorithms();
    static int getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2);
    static double getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c);
    static int getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points);
    static double distance2Points(QPoint3D &p1, QPoint3D &p2);
    static int getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points);
    static std::vector<Edge> DT(std::vector<QPoint3D> &points);
    static QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    static std::vector<Edge> createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz);
    static double calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static double calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static std::vector<Triangle> analyzeDTM(std::vector<Edge> & dt);
};

#endif // ALGORITHMS_H
