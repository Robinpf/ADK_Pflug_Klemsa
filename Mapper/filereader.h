#ifndef FILEREADER_H
#define FILEREADER_H
#include "polygon.h"
#include <QString>
#include <QPoint>


class FileReader
{
public:
    FileReader(std::string fileName);
    std::vector<std::vector<QPoint>> polygonsImport();

private:
    std::string fileName;

};

#endif // FILEREADER_H
