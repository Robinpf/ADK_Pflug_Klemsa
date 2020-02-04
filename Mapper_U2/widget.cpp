#include "widget.h"
#include "ui_widget.h"
#include "filereader.h"
#include "algorithms.h"
#include <QPointF>
#include <QFileDialog>
#include <QApplication>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_drw_points_clicked()
{
    ui->canvas->setDrawMode(DrawMode::P0OINTS);
    ui->canvas->saveActualPolygon();
}


void Widget::on_btn_gen_convex_hull_clicked()
{
    ui->canvas->clearConvexHullPolygon();
    QVector<QPointF> points = ui->canvas->points;
    QElapsedTimer timer;
    ui->console->moveCursor (QTextCursor::End);

    QString text = ui->cmb_convex_hull_alg->currentText();

    if (text == "Jarvis Scan")
    {
        timer.start();
        ui->canvas->convexHull = Algorithms::jarvisScan(points);
    }
    if (text == "Quick Hull")
    {
        timer.start();
        ui->canvas->convexHull = Algorithms::qHull(points);
    }
    if (text == "Sweep Line")
    {
        timer.start();
        ui->canvas->convexHull = Algorithms::sweepLine(points);
    }
    if (text == "Graham Scan")
    {
        timer.start();
        ui->canvas->convexHull = Algorithms::grahamScan(points);
    }
    ui->console->insertPlainText("time for calculate convex hull: " + QString::number(timer.elapsed()) + "milliseconds\n\n");

    repaint();
}


void Widget::on_btn_clear_clicked()
{
    ui->canvas->clearAll();
}

void Widget::on_btn_gen_rand_points_clicked()
{
    int n = ui->spn_rand_points->value();
    QString shp = ui->cmb_rand_shape->currentText();

    Shape shape = CIRCLE;
    shape = shp == "Square" ? SQUARE : shape;
    shape = shp == "Random" ? RANDOM : shape;
    shape = shp == "Raster" ? RASTER : shape;



    QVector<QPointF> points;
    points = Algorithms::generateRandomPoints(n,shape);
    ui->canvas->points = points;
    repaint();
}

void Widget::on_btn_tst_algorithms_clicked()
{
    int n = 1; // repetitons
    int n_max = 10;
    QElapsedTimer timer;

    int i = 0;
    int i_max = 10;//10;
    int sets[i_max] = {1000,5000,10000,25000,50000,75000,100000,250000,500000,750000,1000000};
    QVector<QPointF> points;

    // Circle
    for (i = 0; i < i_max; i++)
    {
        ui->console->insertPlainText("Time for calculate convex hull with " + QString(sets[i]) + " points.\n");

        for (n = 1; n <= n_max; n++)
        {
            points = Algorithms::generateRandomPoints(sets[i],RANDOM);
            timer.start();
            Algorithms::qHull(points);
            ui->console->insertPlainText("time: " + QString::number(timer.elapsed()) + "milliseconds\n");

        }
    }

    //Algorithms
}
