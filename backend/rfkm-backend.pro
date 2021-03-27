TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += sql

SOURCES += \
        dbserver.cpp \
        httpserver.cpp \
        main.cpp \
        requestmanager.cpp

LIBS += -pthread -lws2_32

HEADERS += \
    dbserver.h \
    httplib.h \
    httpserver.h \
    requestmanager.h
