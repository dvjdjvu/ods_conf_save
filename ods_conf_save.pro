TARGET = ods_conf_save
TEMPLATE = lib
LANGUAGE = Qt4/C++
INCLUDEPATH = . \
    src \
    src/forms \
    ../include \
    /usr/local/ods/include \
    /usr/odsAppSrv/include \
    /usr/odsAppSrv/tools/qjson/include
CONFIG += qt \
    release \
    warn_on \
    plugin
QT = core \
    xml \
    xmlpatterns

# UI_DIR = ./src/forms

_OS_ = $$system("[ ! -e /usr/include/mac.h ]")
contains(_OS_, 1) { 
    DEFINES += MCBC=1
}
else { 
    DEFINES += ASTRA=1
}

CONFIG(debug, debug|release) { 
    # Debug
    unix:TARGET = $$join(TARGET,,,_debug)
    else:TARGET = $$join(TARGET,,,d)
    OBJECTS_DIR = ./build/debug/.obj
    MOC_DIR = ./build/debug/.moc
    RCC_DIR = ./build/debug/.rcc
}
else { 
    # Release
    OBJECTS_DIR = ./build/release/.obj
    MOC_DIR = ./build/release/.moc
    RCC_DIR = ./build/release/.rcc
}

SOURCES = src/ods_conf_save.cpp \
    src/ods_conf_save_impl.cpp
HEADERS += src/ods_conf_save.h \
    src/ods_conf_save_impl.h
LIBS += -lOdsInterface \
    -L/usr/local/ods/lib \
    -L/usr/odsAppSrv/tools/qjson/lib \
    -lqjson
DESTDIR += lib
QT -= gui
OPENEDFILES += src/ods_conf_save.h \
    src/ods_conf_save.cpp 
