#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T17:43:00
#
#-------------------------------------------------

QT       -= core gui

TARGET = argon2
TEMPLATE = lib

ARCH = generic

ARCHDIR = arch/$$ARCH

INCLUDEPATH += ../../include ../../lib
INCLUDEPATH +=  ../../$$ARCHDIR/include ../../$$ARCHDIR/lib

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
    ../../lib/blake2/blake2-impl.h \
    ../../lib/blake2/blake2-opt.h \
    ../../lib/blake2/blake2-common.h

equals(ARCH, x86_64) {
    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c \
        ../../$$ARCHDIR/lib/blake2/blake2-arch.c
}
equals(ARCH, generic) {
    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c \
        ../../$$ARCHDIR/lib/blake2/blake2-arch.c
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
