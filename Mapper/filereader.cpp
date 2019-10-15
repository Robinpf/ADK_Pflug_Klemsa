#include "filereader.h"
#include <iostream>
#include <fstream>
#include <QPoint>

FileReader::FileReader(std::string fileName)
{
    this->fileName = fileName;
}

std::vector<std::vector<QPoint>> FileReader::polygonsImport()
{
    std::ifstream imputFile;
    std::string line;
    imputFile.open(this->fileName);

    // Point coordinates
    double x, y;
    QPoint p;
    int num = 0;
    int i = 0;
    std::vector<QPoint> polygon;
    std::vector<std::vector<QPoint>> polygons;

    if (imputFile.is_open())
    {
        while (imputFile >> num >> x >> y)
        {
            i++;
            if (i == num) {
                // Push back actual polygon.
                p.setX(x);
                p.setY(y);
                polygon.push_back(p);
            } else {
                // Create new polygon.
                polygons.push_back(polygon);
                polygon.clear();
                // add new point to new polygon
                p.setX(x);
                p.setY(y);
                polygon.push_back(p);
                i = 1;
            }
            std::cout << x << "   " << y << std::endl;
        }
        // Push back last polygon.
        polygons.push_back(polygon);
    }
    else
    {
        throw "File could not be opened.";
    }

    imputFile.close();
    return polygons;
}
