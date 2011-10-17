######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 9 08:48:58 2011
######################################################################

QT += phonon declarative
CONFIG += qdbus
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
  MEEGO_VERSION_MAJOR     = 1
  MEEGO_VERSION_MINOR     = 2
  MEEGO_VERSION_PATCH     = 0
  MEEGO_EDITION           = harmattan
  DEFINES += MEEGO_EDITION_HARMATTAN
}


# Input
SOURCES += main.cpp \
           manager.cpp \
           adapter.cpp \
           device.cpp \
           proximity.cpp \
	   monitorqml.cpp \
	   monitor.cpp \

HEADERS += types.h \
           manager.h \
           adapter.h \
           device.h \
           proximity.h \
	   monitorqml.h \
	   monitor.h \

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/DeviceListItem.qml \
    qml/MonitorView.qml \
    qml/Slider.qml \
    qml/images/knob.png \
    qml/images/off.png \
    qml/images/list_bg.png \
    qml/images/header.png \
    qml/images/content_bg_signal.png \
    qml/images/content_bg_alerts.png \
    qml/MainMeego.qml \
    uiprox.desktop \
    uiprox.png \
    uiprox.svg

RESOURCES += \
    resource.qrc

include(deployment.pri)
qtcAddDeployment()

unix:!symbian:!maemo5 {
    target.path = /opt/uiprox/bin
    INSTALLS += target
}
