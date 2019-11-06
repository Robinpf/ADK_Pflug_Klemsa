#include "algorithms.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <QPointF>
#include <vector>
#include "draw.h"
#include "algorithms.h"
#include "sortbyy.h"
#include "sortbyx.h"
#include "sortpointsangleq.h"

Algorithms::Algorithms()
{
}


int Algorithms::getPointLinePosition(QPointF &q, QPointF &p1, QPointF &p2)
{

//Tolerance
double eps = 1.0e-6;
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
if (t > eps)
    return 1;

//Point in the right half plane
if (t < -eps)
    return 0;

//Point on the line
return -1;
}

double Algorithms::getPointLineDistance(QPointF &q, QPointF &p1, QPointF &p2){
    //Calculate point and line distance
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumenator = sqrt(dx * dx + dy * dy);

    double dist = fabs(numerator)/denumenator;
    return dist;
}

double Algorithms::getAngle2Vectors(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4)
{
    // Calculate Vector betwen 2 vectors.
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}


int Algorithms::positionPointPolygonWinding(QPointF &q, QPolygonF &pol)
{
    QPointF p_i1;
    QPointF p_i2;

    // Analyze Position of the Point and the Polygon
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-6;

    // The size of polygon
    int n = pol.size();

    //Boundary number
    int boundary = 0;

    //Browse all points of polygon
    for (int i = 0; i < n; i++){

        p_i1 = pol.at(i);
        p_i2 = pol.at((i+1)%n);

        //Measure angle
        double omega = getAngle2Vectors(p_i1, q, p_i2, q);

        //Get orientation of the point and the polygon edge
        int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left half plane
    if (orient == 1){
            wn += omega;
    }

        //Point in the right half plane
    else if (orient == 0){
            wn -= omega;
       }

    //Point on an edge
    else {
        ++boundary;
    }
   }


    //Testing position of point on edge
    if (boundary == 0){

    //point inside polygon
    if (fabs(fabs(wn) - 2 * M_PI) <= eps)
       return 1;

    //Point outside polygon
     else {
        return 0;
      }
    }

    //Point on boundary
    else {
        return -1;
    }
}


int Algorithms::positionPointPolygonRayCrossing(QPointF &q, QPolygonF &pol)
{
    // Analyze Position of the Point and the Polygon
    int k = 0;

    // Size of polygon
    int n = pol.size();

    //Tolerance
    double eps = 1.0e-6;

    //Reduce first point
    double xir = pol.at(0).x() - q.x(); // BUG!!!!!
    double yir = pol.at(0).y() - q.y();

    //All points of polygon
    for (int i = 1; i < n+1 ; i++)
    {
        //Reduce coordinates
        double xiir = pol.at(i%n).x() - q.x();
        double yiir = pol.at(i%n).y() - q.y();

        //Point in the upper half plane
        if ((yir > 0) && (yiir <= 0) || (yiir > 0) && (yir <= 0) )
        {

            // Compute intersection
            double xm = (xir * yiir - xiir * yir)/(yir - yiir);

            // Point in the right plane
        if (xm > eps){
                k +=1;
        }

        // Point in the left
        else if(xm < -eps) {
        k +=0;
        }

        // Point is out of tolerance
        else {

         //Boundary
         return -1;
        }
     }


        //Assign coordinates
        yir = yiir;
        xir = xiir;
    }

    //Odd/even amount of intersections
    return k%2;
}


QPolygonF Algorithms::createRandomPolygon(){

    //declaration
    double x = 0;
    double y = 0;
    double theta = 0;
    double r = 0;
    QVector<QPointF> randPoints;

    //generate random points in i vertex polygon
    for(int i = rand()% 20 + 4; i > 0; i--){
        theta += rand()% (360/i) + 5; //decimal degree
        theta = theta * (M_PI/180); //radians
        r = rand()% 349 + 1; // pruvodic

        //rendering x,y
        x = r * sin(theta) + 350;
        y = r * cos(theta) + 250;
        QPointF newPoint(x,y);

        //Beware for duplicates!
        randPoints.push_back(newPoint);
    }

    QPolygonF polygon(randPoints);
    return polygon;
}

QVector<QPointF> Algorithms::generateRandomPoints(int numberOfPoints, Shape shape)
{
    QPointF center(500.0,275.0);
    QVector<QPointF> points;
    QPointF point(0.0,0.0);
    // Circle.
    double r = 200.0;
    double pi;

    // Square / raster.
    double a = 400.0;
    int n = 50;
    int x,y;

    switch (shape) {
    case CIRCLE:
        for (int i = 1; i < numberOfPoints; i++)
        {
            // Create random point
            pi = ((double)rand() / RAND_MAX) * 2 * M_PI;
            point.setX(center.x() + sin(pi) * r);
            point.setY(center.y() + cos(pi) * r);
            points.push_back(point);
        }
        break;

    case SQUARE:
        for (int i = 1; i < numberOfPoints; i++)
        {
            // Create random point
            x = (((double)rand() / RAND_MAX) * a) - a/2;
            y = (((double)rand() / RAND_MAX) * a) - a/2;

            point.setX(center.x() + x);
            point.setY(center.y() + y);

            if (rand() % 2) {
                x = rand() % 2 ? center.x() - a/2 : center.x() + a/2;
                point.setX(x);
            } else {
                y = rand() % 2 ? center.y() - a/2 : center.y() + a/2;
                point.setY(y);
            }
            points.push_back(point);
        }
        break;

    case GRID:
        for (int i = 1; i < numberOfPoints; i++)
        {
            // Create random point
            x = (((double)rand() / RAND_MAX) * (a+n)) - a/2;
            y = (((double)rand() / RAND_MAX) * (a+n)) - a/2;
/*
            if (rand() % 2)
            {
                x = (((int)x + n/2) / n) * n;
            } else
            {
                y = (((int)y + n/2) / n) * n;
            }
*/

            point.setX(x + center.x() - n);
            point.setY(y + center.y() - n);

            points.push_back(point);
        }

        break;
    case RASTER:
        for (int i = 1; i < numberOfPoints; i++)
        {
            // Create random point
            x = (((double)rand() / RAND_MAX) * (a+n)) - a/2;
            y = (((double)rand() / RAND_MAX) * (a+n)) - a/2;

            x = (((int)x + n/2) / n) * n;
            y = (((int)y + n/2) / n) * n;

            point.setX(x + center.x() - n);
            point.setY(y + center.y() - n);

            points.push_back(point);
        }

        break;
    default:
        break;
    }
    return points;
}

QPolygonF Algorithms::strictlyConvex(QPolygonF &ch)
{

    //Delete points on the same line
    for(int i=0; i<(ch.size()-2); i++){
    if(getPointLinePosition(ch[i+2],ch[i],ch[i+1])==-1){
        ch.remove(i+1);
        i--;
    }
    }

    return ch;
}

QPolygonF Algorithms::jarvisScan(QVector<QPointF> &points)
{
    //Tolerance
    double e = 1.0e-6;
    //Convex hull
    QPolygonF ch;

    //Sort points by Y
    std::sort(points.begin(), points.end(), SortbyY());
    QPointF q=points[0];

    std::sort(points.begin(), points.end(), SortbyX());
    QPointF r=points[0];

    //Initialize points pj, pjj
    QPointF pj = q;
    QPointF pjj (r.x(), q.y());

    // Add to CH
    ch.push_back(q);

    // Find points of CH
    do
    {
    double om_max = 0;
    int i_max = 0;
    double d_min = 1e9;

    //Find point of CH
    for(int i = 0; i < points.size(); i++)
    {
        double omega = getAngle2Vectors(pj, pjj, pj, points[i]);

        // Actualize maximum.
    if (omega > om_max)
    {
    om_max = omega;
    i_max = i;
    }
    // Colinear points
    else if ((omega - om_max) < 0.0001)
    {
    double d = sqrt((pj.x()-points[i].x())*(pj.x()-points[i].x())+(pj.y()-points[i].y())*(pj.y()-points[i].y()));
    if (d_min < d)
    {
        d_min = d;
        om_max = omega;
        i_max = i;
    }
    }
    }

    //Add point to the convex hull
    ch.push_back(points[i_max]);

    //Change index
    pjj = pj;
    pj = points[i_max];

    } while (!(pj == q));

    strictlyConvex(ch);
    return ch;
}

QPolygonF Algorithms::qHull(QVector<QPointF> &points)
{

    QPolygonF ch;
    QVector<QPointF> upoints, lpoints;


    std::sort(points.begin(), points.end(), SortbyX());
    QPointF q1 = points[0];
    QPointF q3 = points[points.size()-1];


    upoints.push_back(q1);
    upoints.push_back(q3);
    lpoints.push_back(q1);
    lpoints.push_back(q3);


    for(unsigned int i = 0; i < points.size(); i++)
    {

    if(getPointLinePosition(points[i], q1, q3) == 1)
    {
        upoints.push_back(points[i]);
    }


    else if(getPointLinePosition(points[i], q1, q3) == 0)
    {
        lpoints.push_back(points[i]);
    }
    }


    ch.push_back(q3);


    qh(1, 0, upoints, ch);

    ch.push_back(q1);

    qh(0, 1, lpoints, ch);

    return ch;
}

void Algorithms::qh (int s, int e, QVector<QPointF> &points, QPolygonF &ch)
{

    int i_max = -1;
    double d_max = -1;

    for(unsigned int i = 2; i < points.size(); i++)
    {
    if(getPointLinePosition(points[i], points[s], points[e]) == 0)
    {
        double d = getPointLineDistance(points[i], points[s], points[e]);

        if(d > d_max)
        {
        d_max = d;
        i_max = i;
        }
    }
    }
    if(i_max > -1)
    {
    qh(s, i_max, points, ch);

    ch.push_back(points[i_max]);

    qh(i_max, e, points, ch);
    }

}


QPolygonF Algorithms::sweepLine(QVector<QPointF> &points)
{
    QPolygonF ch;

    std::sort(points.begin(), points.end(), SortbyX());

    std::vector<QPointF> pointsWithRemovedDuplicit;
    for(unsigned int i =0; i<points.size() - 1; i++){
    if((points[i].x()!=points[i+1].x()) || (points[i].y()!=points[i+1].y())  ){
        pointsWithRemovedDuplicit.push_back(points[i]);
    }
    }
    pointsWithRemovedDuplicit.push_back(points[points.size() - 1]);

    points.clear();
 for (int i = 0; i < pointsWithRemovedDuplicit.size(); i++)
 {
     points.push_back(pointsWithRemovedDuplicit[i]);
 }


    int m = points.size();
    std::vector<int> p(m), n(m);

    n[0] = 1;
    n[1] = 0;

    p[0] = 1;
    p[1] = 0;

    for(unsigned int i = 2; i < points.size(); i++)
    {

    if(getPointLinePosition(points[i], points[p[i-1]], points[i-1]) == 1)
    {
        p[i] = i-1;
        n[i] = n[i-1];
    }
    else
    {
        p[i] = p[i-1];
        n[i] = i-1;
    }

    n[p[i]] = i;
    p[n[i]] = i;

    while(getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
    {
        p[n[n[i]]] = i;
        n[i] = n[n[i]];

    }

    while(getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
    {
        n[p[p[i]]] = i;
        p[i] = p[p[i]];
    }
    }

    ch.push_back(points[0]);

    unsigned int index = n[0];

    while(index != 0)
    {
    ch.push_back(points[index]);
    index = n[index];
    }

    strictlyConvex(ch);

    return ch;
}


QPolygonF Algorithms::grahamScan(QVector<QPointF> &points)
{
    // sort by Y and add point with smallest Y to the convex hull
    std::sort(points.begin(), points.end(), SortbyY());

    // pivot with min y and max x
    QPointF q = points[0];
    points.erase(points.begin());

    QVector<QPointF> sortPoints = sortPointsAngleQ::sortPointsByAngleQ(points, q);

    QPolygonF ch;

    ch.push_back(q);
    ch.push_back(sortPoints[0]);

    //point line position
    for(int i = 1; i < sortPoints.size(); i++)
       {
       while(getPointLinePosition(ch[ch.size()-1], ch[ch.size()-2], sortPoints[i]) == 0 || getPointLinePosition(ch[ch.size()-1], ch[ch.size()-2], sortPoints[i]) == -1)
       {
           ch.pop_back();
       }
    ch.push_back(sortPoints[i]);
       }

    strictlyConvex(ch);
    return ch;

}
