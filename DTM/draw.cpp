#include "draw.h"
#include "algorithms.h"
#include "widget.h"
#include <fstream>
#include <QMessageBox>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw points
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 5, points[i].y() - 5, 10, 10);
    }

    //Draw edges
    QPen p(Qt::green, 1);
    painter.setPen(p);

    for (int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getStart(), dt[i].getEnd());
    }

    //Draw slope
    if (analyzeType == SLOPE) {
        double k = 255.0 / 180;
        for (Triangle t : dtm)
        {
            //Get triangle vertices
            QPoint3D p1 = t.getP1();
            QPoint3D p2 = t.getP2();
            QPoint3D p3 = t.getP3();
            int slope = 255 - t.getSlope() * k;

            //Set color brush
            QColor c(slope, slope, slope);
            painter.setBrush(c);

            //Create polygon
            QPolygonF triangle;
            triangle.append(QPointF(p1.x(), p1.y()));
            triangle.append(QPointF(p2.x(), p2.y()));
            triangle.append(QPointF(p3.x(), p3.y()));

            painter.drawPolygon(triangle);
        }
    }

    //Draw aspect
    if (analyzeType == ASPECT) {
        for (Triangle t : dtm)
        {
            //Get triangle vertices
            QPoint3D p1 = t.getP1();
            QPoint3D p2 = t.getP2();
            QPoint3D p3 = t.getP3();
            int aspect = (int)t.getAspect();

            //Set color brush
            QColor c(0,0,0);
            c.setHsv(aspect,255,255);
            painter.setBrush(c);

            //Create polygon
            QPolygonF triangle;
            triangle.append(QPointF(p1.x(), p1.y()));
            triangle.append(QPointF(p2.x(), p2.y()));
            triangle.append(QPointF(p3.x(), p3.y()));

            painter.drawPolygon(triangle);
        }
    }

    // Draw hypsometry
    if (analyzeType == HYPSOMETRY) {
        for (Triangle t : dtm)
            {
                //Get triangle vertices
                QPoint3D p1 = t.getP1();
                QPoint3D p2 = t.getP2();
                QPoint3D p3 = t.getP3();
                int aspect = (int)t.getAspect();
                QPoint3D lowest;
                QPoint3D highest;

                if (p1.getZ() <= p2.getZ()) {
                    if (p2.getZ() <= p3.getZ()) {
                        // p1 < p2 < p3
                        lowest = p1;
                        highest = p3;
                    } else if (p1.getZ() <= p3.getZ()) {
                        // p1 < p3 < p2
                        lowest = p1;
                        highest = p2;
                    } else {
                        // p3 < p1 < p2
                        lowest = p3;
                        highest = p2;
                    }
                } else {
                    if (p1.getZ() <= p3.getZ()) {
                        // p2 < p1 < p3
                        lowest = p2;
                        highest = p3;
                    } else if (p2.getZ() <= p3.getZ()) {
                        // p2 < p3 < p1
                        lowest = p2;
                        highest = p1;
                    } else {
                        // p3 < p2 < p1
                        lowest = p3;
                        highest = p1;
                    }
                }

                // Calculate pitch point
                double alpha = atan2(lowest.x() - highest.x(), lowest.y()-highest.y())/M_PI*180 ;
                double beta = (alpha - aspect) >= 0 ? (alpha - aspect) : (alpha - aspect) + 360;
                double distance = cos(beta/180*M_PI)*Algorithms::distance2Points(lowest,highest);

                double dx = cos((-aspect+90.0)/180.0*M_PI) * distance;
                double dy = sin((-aspect+90.0)/180.0*M_PI) * distance;
                QPoint3D pitchPoint(lowest.x() - dx, lowest.y() - dy, highest.getZ());

                // Lowest and highest color.
                int h1 = (int)lowest.getZ();
                int h2 = (int)highest.getZ();

                QColor lc(h1,h1,h1);
                QColor hc(h2,h2,h2);

                //Set color brush
                QLinearGradient g(lowest,pitchPoint);
                g.setColorAt(0.0,lc);
                g.setColorAt(1.0,hc);

                painter.setBrush(g);

                //Create polygon
                QPolygonF triangle;
                triangle.append(QPointF(p1.x(), p1.y()));
                triangle.append(QPointF(p2.x(), p2.y()));
                triangle.append(QPointF(p3.x(), p3.y()));

                painter.drawPolygon(triangle);
            }
    }



    //Draw countour lines
    QPen q(Qt::gray, 1);
    painter.setPen(q);

    for (int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates of cursor
    QPoint3D p;
    p.setX(event ->x());
    p.setY(event ->y());
    double random = std::rand() * 255.0 / RAND_MAX;
    p.setZ(random);

    //Add point to the list
    points.push_back(p);

    repaint();
};

void Draw::importPoints(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &z_min, double &z_max)
{


    double x, y, z;
    QPoint3D p;


    //go through file and load points into poly_pol (storing all polygons)
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
    double min_z = std::numeric_limits<double>::max();
    double max_z = std::numeric_limits<double>::min();


    std::ifstream myfile(path);
    if(myfile.is_open())
    {
        qDebug() << "File is open";
        while(myfile >> x >> y >> z)        //read file line by line
        {
            p.setX(x);
            p.setY(y);
            p.setZ(z);

            points.push_back(p);

            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
            if(z < min_z) min_z = z;
            if(z > max_z) max_z = z;
        }

        myfile.close();
    }

    //scale points to canvas size
        double h = canvas_size.height() - 40;
        double w = canvas_size.width() - 40;

        double x_coef = w/(max_x-min_x);
        double y_coef = h/(max_y-min_y);

        for(unsigned int i = 0; i < points.size(); i++)
        {
            points[i].setX((points[i].x()-min_x)*x_coef);
            points[i].setY((points[i].y()-min_y)*y_coef);
        }
}

