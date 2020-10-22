QT -= gui
CONFIG += c++11
VERSION = 0.0.9
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
TARGET = i2cInterface
DEPENDPATH += usr/include/glib-2.0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
SOURCES += \
        i2c.cpp \
        main.cpp

# Default rules for deployment.

target.path = /application/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    i2c.hpp
    packagesExist(glib-2.0) {
        CONFIG += link_pkgconfig
        PKGCONFIG += glib-2.0
    }
