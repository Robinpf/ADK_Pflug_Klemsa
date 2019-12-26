#ifndef SORTBYY_H
#define SORTBYY_H

#include<QtGui>


class SortbyY
{
public:
    bool operator ()(QPointF &p1, QPointF &p2){
        return p1.y() < p2.y();
    }
};

#endif // SORTBYY_H
