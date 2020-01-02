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
    //Point and line position
    double eps = 1.0e-8;

    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    if(t > eps)
        return 1;

    if(t < -eps)
        return 0;

    double eps2 = 2.0;
    double dist_qp1 = sqrt((vx*vx)+(vy*vy));
    double dist_qp2 = sqrt((q.x()-p2.x())*(q.x()-p2.x()) + (q.y()-p2.y())*(q.y()-p2.y()));
    double dist_p1p2 = sqrt((ux*ux)+(uy*uy));
    double dist = dist_qp1 + dist_qp2;

    if((fabs(dist-dist_p1p2))<=eps2){
        return -2;
    }

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
    int n = pol.size();
    double sum_fi=0.0;
    double eps = 1.0e-10;
    int k = 0;

    for (int i=0; i < n; i++)
    {
        if( fabs(q.x()-pol[i].x())<eps && fabs(q.y()-pol[i].y())<eps )
        {
            k = -1;
        }
        else
        {

             double fi = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

             int t = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

             // q point on the line
             if(t==-2)
                 return 1;

             if(t > 0)
                 sum_fi += fi;

             else
                 sum_fi -= fi;
        }
    }

 if(k==-1)
 {
     //Point q on the vertex
     return 1;
 }
 else
 {
    if (fabs(fabs(sum_fi)-360)< eps)
        return 1;

    if (fabs(sum_fi) < eps)
        return 0;

    else
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

    int n_points = (rand()% 20 + 4) -1;

    QPolygonF poly;

    for(int i = 0; i < n_points; i++)
    {
        QPointF point;
        point.setX(rand()%500);
        point.setY(rand()%500);
        poly.push_back(point);
    }

    QPolygonF polygon;

    while(!poly.empty())
    {
        int indexMin = 0;
         for(int i = 1; i < poly.size(); i++)
            {
         if(poly.at(indexMin).y() > poly.at(i).y()){
         indexMin = i;
             }
         }
            polygon<<(poly.takeAt(indexMin));
        }

        QPointF center;

        double prumerX = 0;
        double prumerY = 0;

        for(int i = 0; i < n_points; i++){
            prumerX = prumerX + polygon.at(i).x();
            prumerY = prumerY + polygon.at(i).y();
        }

        center.setX(prumerX/n_points);
        center.setY(prumerY/n_points);

        QPolygonF polygon2;

        while(!polygon.empty()){

            int indMin = 0;
            double uhelMin = 360;
            double uhel = 0;

             for(int i = 0; i < polygon.size(); i++) {
                double smernik = atan(fabs((polygon[i].x()-center.x()))/fabs(polygon[i].y()-center.y()));


                // Check the quadrant and fix the angel
                if((polygon[i].x()-center.x())<0 && (polygon[i].y()-center.y())>0){
                    uhel = smernik;
                }

                if((polygon[i].x()-center.x())<0 && (polygon[i].y()-center.y())<0){
                    uhel = 180-smernik;
                }

                if((polygon[i].x()-center.x())>0 && (polygon[i].y()-center.y())<0){
                    uhel = 180+smernik;
                }

                if((polygon[i].x()-center.x())>0 && (polygon[i].y()-center.y())>0){
                    uhel = 360-smernik;
                }

                // find the smallest so the angels would be in clockwise and final polygon would be topological correct
                if(uhel<uhelMin){
                    uhelMin = uhel;
                    indMin = i;
                }
            }

            // Delete minimum and add it to another polygon so you can count with the rest
            polygon2<<(polygon.takeAt(indMin));
        }

        polygon2<<center;

        return polygon2;
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

            if (rand() % 2)
            {
                x = (((int)x + n/2) / n) * n;
            } else
            {
                y = (((int)y + n/2) / n) * n;
            }


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
    // Delete duplicates
    int m = ch.size();
    for (int j = 1; j < m; j++){
        if (ch[j].x() == ch[(j+1)%m].x() && ch[j].y() == ch[(j+1)%m].y())
        {
            ch.remove(j);
        }
    }

    //Collinear control loop
    int n = ch.size();
    for (int i = 0; i < n-2; i++)
    {
    if (getPointLinePosition(ch[i+2], ch[i], ch[i+1]) == -1)
    {
        ch.remove(i+1);
        i--;
    }
    }

    // Last and firts point are collinear
    if (getPointLinePosition(ch[0], ch[ch.size()-2], ch[ch.size()-1]) == -1)
    {
        ch.remove(ch.size()-1);
    }

    return ch;
}

double Algorithms::getDistance(QPointF &a, QPointF &b)
{
    return sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
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
	double min_dist = std::numeric_limits<double>::max(); //set min_dist for colinear poins

	//Find pi = arg max angle (pi, pj, pjj)
	for(unsigned int i = 0; i<points.size(); i++)
	{
	    //get angle between 2 segments
	    double fi = getAngle2Vectors(pj, pjj, pj, points[i]);

	    //find maximum
	    if(fi > fi_max)
	    {
		i_max = i;
		fi_max = fi;
	    }

	    //in case of colinear points, get one, that is closest to point pj
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

	//add the next point to convex hull
	ch.push_front(points[i_max]);

	//assign convex hull vertices
	pjj = pj;
	pj = points[i_max];
    }
    while(!(pj == q));

    return ch;
}


QPolygonF Algorithms::qHull(QVector<QPointF> &points)
{
    //Convex Hull by quick hull method
    QPolygonF ch;
    QVector<QPointF> upoints, lpoints;

    // Sort by X.
    std::sort(points.begin(), points.end(), SortbyX());
    QPointF q1 = points[0];
    QPointF q3 = points.back();

    //Add q1, q3 to upoints, lpoints
    upoints.push_back(q1);
    upoints.push_back(q3);

    lpoints.push_back(q1);
    lpoints.push_back(q3);

    // Split to upoints / lpoints
    for(unsigned int i=0; i < points.size();i++){

        //Add to the upper part
        if(getPointLinePosition(points[i], q1, q3) == 1)
            upoints.push_back(points[i]);

        //Add to the lower part
        else if (getPointLinePosition(points[i], q1, q3) == 0)
            lpoints.push_back(points[i]);
    }

    //Call recursive function
    ch.push_back(q3);
    qh(1, 0, upoints, ch);
    ch.push_back(q1);
    qh(0, 1, lpoints, ch);

    strictlyConvex(ch);
    return ch;
}

void Algorithms::qh(int s, int e, QVector<QPointF> &points, QPolygonF &ch){

    //Recursive procedure
    int i_max = -1;
    double d_max = 0;

    // Browse all points
    for (int i = 2; i < points.size(); i++)
    {
       // Is the point in the right half plane
        if (getPointLinePosition(points[i], points[s], points[e]) == 0)
        {
            double d = getPointLineDistance(points[i], points[s], points[e]);

            //Actualization of d_max
            if(d > d_max){
                d_max = d;
                i_max = i;
            }
        }
    }

    //Suitable point has been found
    if(i_max != -1)
    {
        qh(s, i_max, points, ch);
        ch.push_back(points[i_max]);
        qh(i_max, e, points, ch);
    }

}


QPolygonF Algorithms::sweepLine(QVector<QPointF> &points)
{
    //Increment method, sweep line
    QPolygonF ch;

    int m = points.size();

    //List of predecessors
    std::vector<int> p(m);

    //List of successors
    std::vector<int> n(m);

    //Sort points by X
    std::sort(points.begin(),points.end(),SortbyX());

    // Create initial aproximation
    n[0] = 1;
    n[1] = 0;
    p[0] = 1;
    p[1] = 0;

    // Process all points according to x coordinates
    for (int i = 2; i < m; i++)
    {
        //Point i lies in the upper half plane
        if(points[i].y() > points[i-1].y()){
            //Link i and its predecessor and successor
            p[i] = i-1;
            n[i] = n[i-1];
        }

        //Point i lies in the lower half plane
        else
        {
            //Link i and its predecessor and successor
            p[i] = p[i-1];
            n[i] = i - 1;
        }

        //Link predecessor and successor and i
        p[n[i]] = i;
        n[p[i]] = i;

        //Fix upper tangent
        while (getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
        {
            //Change predecessor and successor
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Fix lower tangent
        while (getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
        {
            //Change predecessor and successor
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //Convert successors to ch
    ch.push_back(points[0]);

    //Second point of CH
    int index = n[0];

    //Repeat until the first point is found
    while(index != 0)
    {
        //Add to the polygon
        ch.push_back(points[index]);

        //Find its successor
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
