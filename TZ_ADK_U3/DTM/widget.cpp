#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "triangle.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0;
    z_max = 1000;
    dz = 10;

    //Set initial values
    ui->lineEdit->setText(QString::number(z_min));
    ui->lineEdit_2->setText(QString::number(z_max));
    ui->lineEdit_3->setText(QString::number(dz));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->clearDT();

    //Construct DT
    std::vector<QPoint3D> points=ui->Canvas->getPoints();
    std::vector<Edge> dt = Algorithms::DT(points);
    ui->Canvas->setDt(dt);

    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearPoints();
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearPoints();
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    std::vector<Edge> dt;

    //Create triangulation
    if (ui->Canvas->getDtSize() == 0)
    {
        std::vector<QPoint3D> points=ui->Canvas->getPoints();
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
    }

    //Triangulation has been created
    else
    {
        dt = ui->Canvas->getDt();
    }

    //Create contour lines
    std::vector<Edge> contours = Algorithms::createContourLines(dt, z_min, z_max, dz);
    ui->Canvas->setContours(contours);

    repaint();
}

void Widget::on_lineEdit_editingFinished()
{
    //Set z_min
    z_min = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_2_editingFinished()
{
    //Set z_max
    z_max = ui->lineEdit_2->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished()
{
    //Set dz
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_5_clicked()
{
    std::vector<Edge> dt;

    QString ct = ui->comboBox->currentText();

    if (ct == "Slope") {
        ui->Canvas->analyzeType = AnalyzeType::SLOPE;
    }
    else if (ct == "Aspect") {
        ui->Canvas->analyzeType = AnalyzeType::ASPECT;
    }
    else if (ct == "Hypsometry") {
        ui->Canvas->analyzeType = AnalyzeType::HYPSOMETRY;
    }

    //Create triangulation
    if (ui->Canvas->getDtSize() == 0)
    {
        std::vector<QPoint3D> points=ui->Canvas->getPoints();
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
    }

    //Triangulation has been created
    else
    {
        dt = ui->Canvas->getDt();
    }

    //Analyze DTM
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);
    ui->Canvas->setDTM(dtm);
    repaint();
}

void Widget::on_pushButton_6_clicked()
{
    std::vector<QPoint3D> points;
        QSizeF canvas_size = ui->Canvas->size();
        QString path = QFileDialog::getOpenFileName(this,tr("Select file"),"/","Text file (*.txt);;All files (*.*)");
        std::string path_utf8 = path.toUtf8().constData();

        Draw::importPoints(path_utf8, points,canvas_size, z_min, z_max);

        ui->Canvas->setPoints(points);
        ui->Canvas->repaint();

}
