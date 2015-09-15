## CaesiumPH
##### v 0.9.2-beta (build 20150915) - Copyright &copy; Matteo Paonessa, 2015. All Rights Reserved.

----------

> A Caesium version featuring lossless JPEG optimization for photographers and webmasters.

----------

##### REQUIREMENTS
* [mozjpeg](https://github.com/mozilla/mozjpeg)
* [Exiv2](http://www.exiv2.org/)

----------

##### TESTED PLATFORMS
* MacOSX Yosemite (v. 10.10.5)
* Windows 10 64 bit
* ArchLinux

----------

##### INSTALLATION
Binaries can be found at [GitHub](https://github.com/Lymphatus/CaesiumPH/releases/latest)

Linux source code must be compiled using Qt5.5.

----------

##### KNOWN BUGS
- Cancel button does not work while compressing (the job finishes anyway)
- On preferences, the browse button for folders shows up everytime
- "ExifTag" is copied too when copying critical parameters
- Some images don't show in preview. Track why.
- JPEG module is not correctly loaded on OSX bundle
- Remove item does not act well
- The EXIF box has the focus (CTRL+A)

----------

##### TODO
- Translations
- Logging
- Clear preview and EXIF tag box when clearing the list or deleting an item

----------

##### CHANGELOG

###### 0.9.1-beta
Initial release

###### 0.9.2-beta
* FIXED
    * Wrong OS recognition under Linux
* ADDED
    * You can now keep only Copyright, Datetime and Comments in EXIF

For a more detailed list of beta commits, see the [GitHub Repository](https://github.com/Lymphatus/CaesiumPH)

----------

##### RESOURCES
* CaesiumPH website - [http://saerasoft.com/caesium/ph](http://saerasoft.com/caesium/ph)
* CaesiumPH Git Repository - [https://github.com/Lymphatus/CaesiumPH](https://github.com/Lymphatus/CaesiumPH)
* Author website - SaeraSoft - [http://saerasoft.com](http://saerasoft.com)
* Author Twitter [http://twitter.com/MatteoPaonessa](http://twitter.com/MatteoPaonessa)
