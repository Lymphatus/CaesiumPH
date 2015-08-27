#include <exiv2/exiv2.hpp>

#include <QString>
#include <QStringList>

#include <stdlib.h>
#include <assert.h>

QString getExifFromPath(char* filename) {
    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
        assert(image.get() != 0);
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty()) {
            std::string error(filename);
            error += ": No Exif data found in the file";
            //TODO Translate
            return QString("No EXIF info available");
            //throw Exiv2::Error(1, error);
        }
        Exiv2::ExifData::const_iterator end = exifData.end();
        QString tmpExif, final;
        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
            const char* tn = i->typeName();
            /*std::string str = i->key() << " ";
                      << "0x" << std::setw(4) << std::setfill('0') << std::right
                      << std::hex << i->tag() << " "
                      << std::setw(9) << std::setfill(' ') << std::left
                      << (tn ? tn : "Unknown") << " "
                      << std::dec << std::setw(3)
                      << std::setfill(' ') << std::right
                      << i->count() << "  "
                      << std::dec << i->value()
                      << "\n";*/
            tmpExif = "<span style='color:#1cb495'>" + QString::fromStdString(i->key()).split(".").at(2) + "</span>&emsp;" +
                    //QString::number(i->tag()) + "\t" +
                    //(tn ? tn : "Unknown") + "\t" +
                    //QString::number(i->count()) + "\t" +
                    QString::fromStdString(i->value().toString()) + "<br />";
            final.append(tmpExif);
        }
        return final;
    }
    catch (Exiv2::Error& e) {
        std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
        //TODO Translate
        return QString("Error while reading EXIF");
    }
}
