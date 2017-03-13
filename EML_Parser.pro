TEMPLATE = app
CONFIG += c++11
CONFIG += static

DEFINES += QT_DEPRECATED_WARNINGS
QT += widgets sql

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"

QMAKE_CXXFLAGS += /MT

#QMAKE_CFLAGS_RELEASE    -= -O2 -MD
#QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -O2 -MD -Zi
#QMAKE_CFLAGS_DEBUG      -= -Zi -MDd

#QMAKE_CFLAGS_RELEASE    += -O2 -MT
#QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += -O2 -MT -Zi
#QMAKE_CFLAGS_DEBUG      += -Zi -MTd

SOURCES += main.cpp \
    base64.cpp \
    content_parser.cpp \
    regex_set.cpp \
    decoder.cpp \
    eml.cpp \
    mainwindow.cpp \
    md5.cpp \
    database.cpp

HEADERS += \
    base64.h \
    content_parser.h \
    regex_set.h \
    decoder.h \
    eml.h \
    mainwindow.h \
    md5.h \
    database.h
