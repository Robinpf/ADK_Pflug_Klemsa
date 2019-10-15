#ifndef POLYGON_H
#define POLYGON_H
#include "geometrical.h"
#include "point.h"
#include <vector>


class Polygon : Geometrical
{
public:
    Polygon(std::vector<Point> polygon);

private:
    std::vector<Point> polygon;
};

#endif // POLYGON_H
