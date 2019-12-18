#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T11:40:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EyePlay
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
	estructurasutiles.cpp \
        emisor.cpp \
        juegomusical.cpp \
        juegorebote.cpp \
        main.cpp \
	menu.cpp \
        util.cpp

HEADERS += \
	estructurasutiles.h \
        emisor.h \
        juegomusical.h \
        juegorebote.h \
        menu.h \
        util.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += "C:\OpenCV\OpenCV4.1.1G\include"
LIBS += -L"C:\OpenCV\OpenCV4.1.1G\lib"\
 -llibopencv_world411

DISTFILES += \
    resources/nota0.png \
    resources/nota0_azul.png \
    resources/nota0_oro.png \
    resources/nota0_rojo.png \
    resources/nota0_verde.png \
    resources/nota1.png \
    resources/nota1_azul.png \
    resources/nota1_oro.png \
    resources/nota1_rojo.png \
    resources/nota1_verde.png \
    resources/nota2.png \
    resources/nota2_azul.png \
    resources/nota2_oro.png \
    resources/nota2_rojo.png \
    resources/nota2_verde.png \
    resources/nota3.png \
    resources/nota3_azul.png \
    resources/nota3_oro.png \
    resources/nota3_rojo.png \
    resources/nota3_verde.png \
    resources/tambor1.png

RESOURCES +=
