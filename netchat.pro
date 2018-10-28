QT       += core gui macextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

TARGET = Netchat
TEMPLATE = app

SOURCES += src/main.cpp \
           src/netchat.cpp \
           src/database.cpp \
           src/options.cpp \
           src/encryption.cpp \
    	   src/configuration.cpp

HEADERS  += src/netchat.h \
           src/database.h \
           src/options.h \
           src/encryption.h \
    	   src/configuration.h

# Disabled because of manual changes in ui_netchat.h
# FORMS    += netchat.ui

macx: ICON = res/icon.icns
win32: RC_FILE = res/icon.rc

RESOURCES += resources.qrc

macx: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lbotan-2

INCLUDEPATH += $$PWD/../../../../../../usr/local/include/botan-2
DEPENDPATH += $$PWD/../../../../../../usr/local/include/botan-2
