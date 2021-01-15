QT += quick

CONFIG += c++14 sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        glalgo.cpp \
        glbitset.cpp \
        glnaive.cpp \
        lifecontroller.cpp \
        main.cpp \
        rlereader.cpp \
        samplescatalog.cpp \
        sampleview.cpp \
        universeview.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    GLife_uk_UA.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    bitmat.h \
    glalgo.h \
    glbitset.h \
    glnaive.h \
    lifecontroller.h \
    rlereader.h \
    samplescatalog.h \
    sampleview.h \
    universeview.h


TARGETDIR = ''

macx {
  TARGETDIR      += $$OUT_PWD/$${TARGET}.app/Contents/MacOS
}
else {
  TARGETDIR      += $$OUT_PWD
}

mkdata.commands = $(MKDIR) $${TARGETDIR}/rle
copydata.commands = $(COPY_DIR) $$PWD/rle $${TARGETDIR}

first.depends = $(first) mkdata copydata
export(first.depends)
export(mkdata.commands)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first mkdata copydata
