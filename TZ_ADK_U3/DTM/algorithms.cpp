#include "algorithms.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2)
{
    //Analyze point and line position
    //1 point in the left half plane
    //0 point in the right half plane
    //-1 point on the line
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t>0)
        return 1;
    if (t<0)
        return 0;
    return -1;
}


double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c){

    //Return radius of the circle given by 3 points
    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double x3 = p3.x();
    double y3 = p3.y();

    //Calculate coeficients k1-k12
    double k1 = x1*x1 + y1*y1;
    double k2 = x2*x2 + y2*y2;
    double k3 = x3*x3 + y3*y3;
    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;
    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;
    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Midpoint of the circle
    double m_numerator = k12 * (-k4) + k11 * k5 - (k10 + k4*k5)*k6;
    double m_denominator = x3 * (-k4) + x2 * k5 + x1 * (-k6);
    double m = 0.5 * m_numerator / m_denominator;

    double n_numerator = k1 * (-k9) + k2 * k8 + k3 * (-k7);
    double n_denominator = y1 * (-k9) + y2 * k8 + y3 * (-k7);
    double n = 0.5 * n_numerator / n_denominator;

    //Set circle center
    c.setX(m);
    c.setY(n);

    //Radius of the circle
    return sqrt((x1 - m) * (x1 - m) + (y1 - n) * (y1 - n));
}

double Algorithms::distance2Points(QPoint3D &p1, QPoint3D &p2)
{
     // Get distance of two points
     double dx = (p1.x() - p2.x());
     double dy = (p1.y() - p2.y());

     return sqrt(dx*dx + dy*dy);
}

int Algorithms::getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    // Find nearest point
    int i_min = 1;
    double d_min = distance2Points(p, points[1]);

    //Browses all points
    for (unsigned int i = 2; i < points.size(); i++)
    {
        double d = distance2Points(p, points[i]);

        //Actualize minimum i and distance
        if (d < d_min)
        {
            d_min=d;
            i_min=i;
        }
    }

    return i_min;
}

int Algorithms::getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all points
    for (unsigned int i = 0; i < points.size(); i++ )
    {
        //Test if points[i] is different from start, end
        if((s != points[i]) && (e != points[i]))
        {
            //Points[i] in the left half plane
            if(getPointLinePosition(points[i], s, e) == 1)
            {
                //Get circle radius
                QPoint3D c;
                double r = getCircleRadius(s, e, points[i], c);

                //Circle center in the right half plane
                if (getPointLinePosition(c, s, e) == 0)
                    r = -r;

                //Actualize minimum
                if(r < r_min)
                {
                    r_min = r;
                    i_min = i;
                }
            }
        }
    }

    return i_min;
}

std::vector<Edge> Algorithms::DT(std::vector<QPoint3D> &points)
{
    //Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort points by X
    sort(points.begin(), points.end(), SortbyX());

    //Pivot
    QPoint3D q = points[0];

    //Find nearest point
    int index = getNearestpoint(q, points);
    QPoint3D qn = points[index];

    //First Delaunay edge
    Edge e1(q, qn);

    //Find optimal Delaunay point
    int i_o = getDelaunayPoint(q, qn, points);

    //No suitable point
    if (i_o == -1)
    {
        //Change orientation
        e1.changeOrientation();

        //Find optimal Delaunay point
        i_o = getDelaunayPoint(e1.getStart(), e1.getEnd(), points);
    }

    //Third point of the triangle
    QPoint3D q3 = points[i_o];

    //Create initial triangle
    Edge e2(e1.getEnd(), q3);
    Edge e3(q3, e1.getStart());

    //Add first triangle to DT
    dt.push_back(e1);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to active edges list
    ael.push_back(e1);
    ael.push_back(e2);
    ael.push_back(e3);

    //Repeat until any triangle exists
    while (!ael.empty())
    {
        Edge ed1=ael.back();
        ael.pop_back();

        //Change orientation of edge
        ed1.changeOrientation();

        //Find optinal Delaunay point
        int i2 = getDelaunayPoint(ed1.getStart(), ed1.getEnd(), points);

        if (i2 != -1)
        {
            // Third point of the triangle
            QPoint3D q3n = points[i2];

            //Create new triangle
            Edge ed2(ed1.getEnd(), q3n);
            Edge ed3(q3n, ed1.getStart());

            //Add triangle to DT
            dt.push_back(ed1);
            dt.push_back(ed2);
            dt.push_back(ed3);

            //Change orientation
            ed2.changeOrientation();
            ed3.changeOrientation();

            //Are ed2 and ed3 in active edges list
            std::list<Edge>::iterator ie2 = find(ael.begin(), ael.end(), ed2);
            std::list<Edge>::iterator ie3 = find(ael.begin(), ael.end(), ed3);

            //Add edge ed2 to active edge list
            if (ie2 == ael.end())
            {
                ed2.changeOrientation();
                ael.push_back(ed2);
            }

            //Edge ed2 already in active edge list, erase
            else ael.erase(ie2);

            //Add edge ed3 to active edge list
            if (ie3 == ael.end())
            {
                ed3.changeOrientation();
                ael.push_back(ed3);
            }

            //Edge ed3 already in active edge list, erase
            else ael.erase(ie3);
        }
    }

    return  dt;
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Compute contour point
    double x = (p2.x() - p1.x())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double y = (p2.y() - p1.y())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();
    QPoint3D A(x, y, z);

    return A;
}

std::vector<Edge> Algorithms::createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Generate contour lines
    std::vector<Edge> contours;

    //Browse all triangles
    for (unsigned int i=0; i<dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        // Find all plane - triangle intersections.
        for (double z = z_min; z <= z_max; z += dz)
        {
            // Get height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Test criteria
            double t12 = dz1 * dz2;
            double t23 = dz2 * dz3;
            double t31 = dz3 * dz1;

            //Triangle in the plane(coplanar)
            if((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge e12 of the triangle in the plane
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge e23 of the triangle in the plane
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge e31 of the triangle in the plane
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Egdes e12, e13 are intersected
            else if ((t12 < 0 && t23 <= 0) || (t12 <= 0 && t23 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p1, p2, z);
                QPoint3D b = getContourPoint(p2, p3, z);

                // Create fragment of contour line.
                Edge e(a, b);
                contours.push_back(e);
            }

            //Egdes e23, e31 are intersected
            else if ((t23 < 0 && t31 <= 0) || (t23 <= 0 && t31 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p2, p3, z);
                QPoint3D b = getContourPoint(p3, p1, z);

                // Create fragment of contour line.
                Edge e(a, b);
                contours.push_back(e);
            }

            //Egdes e31, e12 are intersected
            else if ((t31 < 0 && t12 <= 0) || (t31 <= 0 && t12 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p3, p1, z);
                QPoint3D b = getContourPoint(p1, p2, z);

                // Create fragment of contour line
                Edge e(a, b);
                contours.push_back(e);
            }
        }
    }

    return contours;
}

double Algorithms::calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);
    double nz = ux * vy - uy * vx;
    double nt = sqrt(nx * nx + ny * ny + nz * nz);

    return acos(nz / nt) / M_PI * 180;
}

double Algorithms::calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);

    double aspect = (atan2(nx, ny) / M_PI * 180);

    return aspect >= 0 ? aspect : aspect + 360 ;
}

std::vector<Triangle> Algorithms:: analyzeDTM(std::vector<Edge> & dt)
{
    //Analyze DTM compute slope and aspect
    std::vector<Triangle> triangles;

    //Browse triangulation one by one
    for (int i = 0; i < dt.size(); i += 3)
    {
        //Vertices of the triangle
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Slope and aspect
        double slope = calculateSlope(p1, p2, p3);
        double aspect = calculateAspect(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, slope, aspect);

        //Add triangle to vector
        triangles.push_back(t);
    }

    return triangles;
}

