#ifndef EXIF_H
#define EXIF_H

#include <stdlib.h>
#include <QString>
#include <exiv2/exiv2.hpp>

QString getExifFromPath(char* filename);

#endif // EXIF_H

