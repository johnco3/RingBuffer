# Define expandable environment variables
EXTLIBS = $$(EXTLIBS_ROOT)
VCPKG = $$(VCPKG_ROOT)

QT       += core-private gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++latest

INCLUDEPATH += .. \
    $${VCPKG}/installed/x64-windows/include \
    $${EXTLIBS}/ait664/include

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

# The following keeps the generated files at least somewhat separate from the source files.
UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
