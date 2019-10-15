#include "widget.h"
#include "ui_widget.h"
#include "filereader.h"
#include "algorithms.h"
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

void Widget::on_btn_drw_polygon_clicked()
{
    ui->canvas->setDrawMode(DrawMode::POLYGON);
}

void Widget::on_btn_anl_polygon_point_clicked()
{
    std::vector<std::vector<QPoint>> polygons;
    polygons = ui->canvas->getPolygons();
    QPoint q = ui->canvas->getAnalyzePoint();
    std::vector<QPoint> pol;

    if (ui->cmb_poly_point_alg->currentText() == "Ray Crossing Algorithm")
    {
        for (int i = 0; i < polygons.size(); i++) {
            pol = polygons[i];
            switch (Algorithms::positionPointPolygonRayCrossing(q,pol)) {
            case 0: // Outside
                //ui->canvas->dr
                break;
            case 1: // Inside

                break;
            case -1: // On boundary

                break;
            default:
                break;
            }
        }
    } else if (ui->cmb_poly_point_alg->currentText() == "Winding Number Algorithm") {
        for (int i = 0; i < polygons.size(); i++) {
            pol = polygons[i];
            switch (Algorithms::positionPointPolygonWinding(q,pol)) {
            case 0: // Outside

                break;
            case 1: // Inside

                break;
            case -1: // On boundary

                break;
            default:
                break;
            }
        }
    }
}
