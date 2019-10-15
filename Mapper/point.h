#ifndef POINT_H
#define POINT_H
#include "geometrical.h"


class Point : Geometrical
{
public:
    Point(double x, double y);

private:
    // Point coordinates.
    double x,y;
};

#endif // POINT_H
