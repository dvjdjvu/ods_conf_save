PACKAGE="Package: libOdsConfSave"
VERSION="Version: 1.0"
SECTION="Section: libs"
Architecture="Architecture: amd64"
Maintainer="Maintainer: Jessy James <djvu@inbox.ru>"
Description="Description: Модуль сохранения настроек приложений"

DEB=libOdsConfSave
DEBIAN=$DEB/DEBIAN

make clean && make

mkdir -p $DEB/usr/odsAppSrv/plugins
mkdir -p $DEB/usr/share/doc/$DEB

cp -r lib/libods_conf_save.so $DEB/usr/odsAppSrv/plugins

mkdir -p $DEBIAN/
touch $DEBIAN/control

echo $PACKAGE > $DEBIAN/control
echo $VERSION >> $DEBIAN/control
echo $SECTION >> $DEBIAN/control
echo $Architecture >> $DEBIAN/control
echo $Maintainer >> $DEBIAN/control
echo $Description >> $DEBIAN/control


dpkg-deb --build $DEB

