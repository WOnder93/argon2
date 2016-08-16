#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T17:43:00
#
#-------------------------------------------------

QT       -= core gui

TARGET = argon2
TEMPLATE = lib

INCLUDEPATH += ../../include ../../lib

SOURCES += \
    ../../lib/argon2.c \
    ../../lib/core.c \
    ../../lib/encoding.c \
    ../../lib/genkat.c \
    ../../lib/impl-select.c \
    ../../lib/thread.c \
    ../../lib/blake2/blake2.c

HEADERS += \
    ../../include/argon2.h \
    ../../include/argon2-opt.h \
    ../../lib/core.h \
    ../../lib/encoding.h \
    ../../lib/genkat.h \
    ../../lib/impl-select.h \
    ../../lib/thread.h \
    ../../lib/blake2/blake2.h \
    ../../lib/blake2/blake2-impl.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/release/ -largon2-arch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/debug/ -largon2-arch
else:unix: LIBS += -L$$OUT_PWD/../libargon2-arch/ -largon2-arch
