#include "cimageinfo.h"
#include "utils.h"

#include <QFileInfo>

CImageInfo::CImageInfo(QString path) {
    QFileInfo* fi = new QFileInfo(path);
    fullPath = path;
    baseName = fi->completeBaseName();
    size = fi->size();
    formattedSize = formatSize(size);
}

CImageInfo::CImageInfo() {

}

CImageInfo::~CImageInfo() {

}

QString CImageInfo::getFullPath() const
{
    return fullPath;
}

void CImageInfo::setFullPath(const QString &value)
{
    fullPath = value;
}
QString CImageInfo::getBaseName() const
{
    return baseName;
}

void CImageInfo::setBaseName(const QString &value)
{
    baseName = value;
}
int CImageInfo::getSize() const
{
    return size;
}

void CImageInfo::setSize(int value)
{
    size = value;
}
QString CImageInfo::getFormattedSize() const
{
    return formattedSize;
}

void CImageInfo::setFormattedSize(const QString &value)
{
    formattedSize = value;
}


