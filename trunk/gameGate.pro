#-------------------------------------------------
#
# Project created by QtCreator 2011-10-06T15:53:18
#
#-------------------------------------------------

# ATTENTION: This application is only designed for Windows.

QT       += core gui network
CONFIG += network

TARGET = custom-tcp-passthrough
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


win32 {
    RC_FILE = MyApp.rc
    WINSDK_DIR = C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A
    WIN_PWD = $$replace(PWD, /, \\)
    OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)
    QMAKE_POST_LINK = "$$WINSDK_DIR/bin/mt.exe -manifest $$quote($$WIN_PWD\\$$basename(TARGET).manifest) -outputresource:$$quote($$OUT_PWD_WIN\\${DESTDIR_TARGET};1)"


}


documentation.path = $$OUT_PWD_WIN\\docs
documentation.files = docs\\*
INSTALLS += documentation


#copyfiles.commands = @echo THIS WILL B $$DESTDIR\\docs docs\\about.txt
#QMAKE_EXTRA_TARGETS += copyfiles
#POST_TARGETDEPS += copyfiles



#install_it.path = $$BUILDDIR/docs
#install_it.files += $$SOURCEDIR/docs/about.txt

#INSTALLS += install_it

OTHER_FILES += \
    MyApp.rc \
    custom-tcp-passthrough.manifest
