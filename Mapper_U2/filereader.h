#ifndef FILEREADER_H
#define FILEREADER_H
#include <QString>
#include <QPoint>
#include <vector>
#include <QPolygonF>


class FileReader
{
public:
    FileReader(std::string fileName);
    QVector<QPolygonF> polygonsImport();

private:
    std::string fileName;

};

#endif // FILEREADER_H
