#include "sortpointsangleq.h"
#include <QtGui>
#include <cmath>

sortPointsAngleQ::sortPointsAngleQ()
{

}

QVector<QPointF> sortPointsAngleQ::sortPointsByAngleQ(QVector<QPointF> &points, QPointF &q){

    QPolygonF poly;

	for(unsigned int j =0; j < points.size(); j++){
	    poly.push_back(points[j]);
	}


    QPolygonF pointsByAngle;

	// sort by angle
	while(!poly.empty()){

	    int ind_max = -1;
	    double fi_max = 0;
	    double fi = 0;

	    for (int i =0; i<poly.size(); i++){

		double alfa = (atan(fabs((q.x()-poly[i].x()))/fabs(q.y()-poly[i].y())))*180/M_PI;


		// Check the quadrant and fix the angel according to the axis X
		if((poly[i].x()-q.x())<0 && (poly[i].y()-q.y())>0){
		    fi = 90 + alfa;
		}

		if((poly[i].x()-q.x())<0 && (poly[i].y()-q.y())<0){
		    fi = 270 - alfa;
		}

		if((poly[i].x()-q.x())>0 && (poly[i].y()-q.y())<0){
		    fi = 270 + alfa;
		}

		if((poly[i].x()-q.x())>0 && (poly[i].y()-q.y())>0){
		    fi = 90 - alfa;
		}


		// points with the same angel

		if(fabs((fi-fi_max))<1.0e-6){
		    double distNew = sqrt((q.x()-poly[i].x())*(q.x()-poly[i].x()) + (q.y()-poly[i].y())*(q.y()-poly[i].y()));
		    double distOld = sqrt((q.x()-poly[ind_max].x())*(q.x()-poly[ind_max].x()) + (q.y()-poly[ind_max].y())*(q.y()-poly[ind_max].y()));

		    if(distOld<distNew){
			poly.remove(ind_max);
			ind_max = i;

		    }
		    else{
			poly.remove(i);

		    }
		    i--;

		}


		// find the biggest angle with axis X
		if(fi>fi_max){
		    fi_max = fi;
		    ind_max = i;
		}

	    }

	    pointsByAngle<<(poly.takeAt(ind_max));
	}

    QVector<QPointF> sortPoints;
	for (int i = 0; i < pointsByAngle.size(); i++){
		sortPoints.push_back(pointsByAngle[i]);
	}

	return sortPoints;

}

