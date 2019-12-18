#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "types.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    ui->Canvas->changePolygon();
}

void Widget::on_pushButton_2_clicked()
{
    //Get polygons
    std::vector<QPointFB> polA = ui->Canvas->getA();
    std::vector<QPointFB> polB = ui->Canvas->getB();

    //Perform Boolean operation
    TBooleanOperation oper = (TBooleanOperation)(ui->comboBox->currentIndex());
    std::vector<Edge> res = Algorithms::booleanOperations(polA, polB, oper);

    //Set results and update
    ui->Canvas->setRes(res);
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearResults();
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();
    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    std::vector<QPointFB> A, B;

    QSizeF canvas_size = ui->Canvas->size();

    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");

    std::string path_utf8 = path.toUtf8().constData();

    QString msg;


   Draw::importPolygons(path_utf8, A, B, canvas_size);

   ui->Canvas->setA(A);

   ui->Canvas->setB(B);

   ui->Canvas->repaint();
}
