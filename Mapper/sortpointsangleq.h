#ifndef SORTPOINTSANGLEQ_H
#define SORTPOINTSANGLEQ_H
#include <QtGui>


class sortPointsAngleQ
{
public:
	sortPointsAngleQ();
    static QVector<QPointF> sortPointsByAngleQ(QVector<QPointF> &points, QPointF &q);
};

#endif // SORTPOINTSANGLEQ_H
