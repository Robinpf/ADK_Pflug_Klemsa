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

    case RANDOM:
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
    const double EPS = 10e-6;

    QPolygonF ch;

    std::sort(points.begin(), points.end(), SortbyY());
    QPointF q = points[0];

    std::sort(points.begin(), points.end(), SortbyX());
    QPointF s = points[0];

    if(fabs(s.x()-q.x())< EPS)
    s.setX(s.x()+100);

    QPointF pjj(s.x(), q.y());
    QPointF pj = q;

    do
    {
    int i_max = -1;
    double fi_max = 0;
    double min_dist = std::numeric_limits<double>::max();
    for(unsigned int i = 0; i<points.size(); i++)
    {
        double fi = getAngle2Vectors(pj, pjj, pj, points[i]);

        if(fi > fi_max)
        {
        i_max = i;
        fi_max = fi;
        }

        else if(fabs(fi-fi_max) < EPS)
        {
        double dist = getDistance(pj, points[i]);
        if(min_dist > dist)
        {
            min_dist = dist;
            i_max = i;
            fi_max = fi;
        }
        }
    }

    ch.push_front(points[i_max]);

    pjj = pj;
    pj = points[i_max];
    }
    while(!(pj == q));

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

double Algorithms::getDistance(QPointF &a, QPointF &b)
{
    return sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
}
