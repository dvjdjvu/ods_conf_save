DEB=libOdsConfSave
DEBIAN=$DEB/DEBIAN

make clean && make

mkdir -p $DEB/usr/odsAppSrv/plugins
mkdir -p $DEB/usr/share/doc/$DEB

cp -r lib/libods_conf_save.so $DEB/usr/odsAppSrv/plugins

mkdir -p $DEBIAN/
touch $DEBIAN/control

dpkg-deb --build $DEB

