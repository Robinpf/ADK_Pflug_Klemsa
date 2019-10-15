#include "algorithms.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <QPointF>
#include <vector>
#include "draw.h"
#include "algorithms.h"

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





