#ifndef GEOMOBJECTS_H
#define GEOMOBJECTS_H
#include <vector>
#include "geometrical.h"


class GeomObjects
{
public:
    static GeomObjects getInstance();
private:
    GeomObjects();

    static GeomObjects GeomObjects = GeomObjects();
    std::vector<Geometrical> geometrical;
};

#endif // GEOMOBJECTS_H
