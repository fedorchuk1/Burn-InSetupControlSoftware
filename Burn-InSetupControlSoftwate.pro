#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T11:10:00
#
#-------------------------------------------------

QT       += core gui widgets
QT       += network

TARGET = Burn-InSetupControlSoftwate
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

#unix:
LIBS += -L/usr/local/lib64/ -lvisa

#unix:
INCLUDEPATH += /usr/local/include

SOURCES += \
    External/FP50ComHandler.cpp \
    External/JulaboFP50.cpp \
    External/VJulaboFP50.cpp \
    GUI/additionalwindow.cpp \
    GUI/mainwindow.cpp \
    VoltageControl/controlkeithleypower.cpp \
    VoltageControl/controlttipower.cpp \
    VoltageControl/powercontrolclass.cpp \
    additionalthread.cpp \
    connectioninterfaceclass.cpp \
    daqcontrolclass.cpp \
    databaseinterfaceclass.cpp \
    environmentcontrolclass.cpp \
    main.cpp \
    systemcontrollerclass.cpp

HEADERS += \
    External/FP50ComHandler.h \
    External/JulaboFP50.h \
    External/VJulaboFP50.h \
    GUI/additionalwindow.h \
    GUI/mainwindow.h \
    VoltageControl/controlkeithleypower.h \
    VoltageControl/controlttipower.h \
    VoltageControl/powercontrolclass.h \
    additionalthread.h \
    connectioninterfaceclass.h \
    daqcontrolclass.h \
    databaseinterfaceclass.h \
    environmentcontrolclass.h \
    systemcontrollerclass.h

FORMS += \
    GUI/additionalwindow.ui \
    GUI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
