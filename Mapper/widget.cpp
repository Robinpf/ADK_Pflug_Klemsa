#include "widget.h"
#include "ui_widget.h"
#include "filereader.h"
#include "algorithms.h"
#include <QPointF>
#include <QFileDialog>


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

void Widget::on_btn_imp_polygons_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);

    // Console write.
    ui->console->moveCursor (QTextCursor::End);
    ui->console->insertPlainText("Importing polygons from file: " + fileName + "\n\n");
    try
    {
        FileReader fileReader(fileName.toStdString());
        ui->canvas->setPolygons(fileReader.polygonsImport());
    } catch (const char* e) {
        ui->console->moveCursor (QTextCursor::End);
        ui->console->insertPlainText(QString::fromStdString(e) + "\n\n");
    }
}

void Widget::on_btn_drw_none_clicked()
{
    ui->canvas->setDrawMode(DrawMode::NONE);
    ui->canvas->saveActualPolygon();
}

void Widget::on_btn_drw_point_clicked()
{
    ui->canvas->setDrawMode(DrawMode::ANALYZE_POINT);
    ui->canvas->saveActualPolygon();
}

void Widget::on_btn_drw_points_clicked()
{
    ui->canvas->setDrawMode(DrawMode::POINTS);
    ui->canvas->saveActualPolygon();
}

void Widget::on_btn_drw_polygon_clicked()
{
    ui->canvas->setDrawMode(DrawMode::POLYGON);
}

void Widget::on_btn_anl_polygon_point_clicked()
{
    ui->canvas->clearHighlitedPolygons();
    QVector<QPolygonF> polygons;
    polygons = ui->canvas->getPolygons();
    QPointF q = ui->canvas->getAnalyzePoint();
    QPolygonF pol;
    int position;

    if (ui->cmb_poly_point_alg->currentText() == "Ray Crossing Algorithm")
    {
        for (int i = 0; i < polygons.size(); i++)
        {
            pol = polygons[i];
            position = Algorithms::positionPointPolygonRayCrossing(q,pol);
            switch (position) {
            case 1: // inside
                ui->canvas->addFilledPolygon(pol);
                break;
            case 0: // outside
                // Do nothing
                break;
            case -1: // on boundary
                ui->canvas->addBorderPolygon(pol);
                break;
            default:
                break;
            }
        }
    } else if (ui->cmb_poly_point_alg->currentText() == "Winding Number Algorithm") {
        for (int i = 0; i < polygons.size(); i++)
        {
            pol = polygons[i];
            position = Algorithms::positionPointPolygonWinding(q,pol);
            switch (position) {
            case 1: // inside
                ui->canvas->addFilledPolygon(pol);
                break;
            case 0: // outside
                // Do nothing
                break;
            case -1: // on boundary
                ui->canvas->addBorderPolygon(pol);
                break;
            default:
                break;
            }
        }
    }
    repaint();
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
    ui->console->insertPlainText("time for calculate convex hull: " + QString::number(timer.elapsed()) + "milliseconds\n\n");

    repaint();
}

void Widget::on_btn_gen_rand_polygon_clicked()
{
    ui->canvas->saveActualPolygon();
    QPolygonF randPolygon;
    randPolygon = Algorithms::createRandomPolygon();
    ui->canvas->setActualPolygon(randPolygon);
    ui->canvas->saveActualPolygon();
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
    shape = shp == "Grid" ? GRID : shape;
    shape = shp == "Raster" ? RASTER : shape;



    QVector<QPointF> points;
    points = Algorithms::generateRandomPoints(n,shape);
    ui->canvas->points = points;
    repaint();
}
