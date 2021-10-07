QT += quick

CONFIG += c++14 sdk_no_version_check

SOURCES += \
        directorysampledatasource.cpp \
        extrledirectorysamplesprovider.cpp \
        extrlesampleconverter.cpp \
        glalgo.cpp \
        glnaive.cpp \
        lifecontroller.cpp \
        main.cpp \
        sample.cpp \
        samplecatalogmodel.cpp \
        samplesprovider.cpp \
        sampleview.cpp \
        universe.cpp \
        universeview.cpp


RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    abstractsampleconverter.h \
    abstractsampledatasource.h \
    borderrule.h \
    directorysampledatasource.h \
    extrledirectorysamplesprovider.h \
    extrlesampleconverter.h \
    glalgo.h \
    glnaive.h \
    lifecontroller.h \
 #   rlereader.h \
    sample.h \
    samplecatalogmodel.h \
    sampledata.h \
#    samplescatalog.h \
    samplesprovider.h \
    sampleview.h \
    universe.h \
    universeview.h


TARGETDIR = ''

macx {
  TARGETDIR      += $$OUT_PWD/$${TARGET}.app/Contents/MacOS
}
else {
  TARGETDIR      += $$OUT_PWD
}

win32 {
    CONFIG -= debug_and_release
}

mkdata.commands = $(CHK_DIR_EXISTS) \"$${TARGETDIR}/rle\" $(MKDIR) \"$${TARGETDIR}/rle\"
copydata.commands = $(COPY_DIR) \"$$PWD/rle\" \"$${TARGETDIR}/rle\"

first.depends = $(first) mkdata copydata
export(first.depends)
export(mkdata.commands)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first mkdata copydata
