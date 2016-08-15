#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T17:43:00
#
#-------------------------------------------------

QT       -= core gui

TARGET = argon2
TEMPLATE = lib

isEmpty(ARCH) {
    warning("ARCH not set, using 'generic'!")
    ARCH = generic
}

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
    ../../lib/blake2/blake2-impl.h

equals(ARCH, x86_64) {
    USE_SSE2 {
        QMAKE_CFLAGS += -msse2
        DEFINES += HAVE_SSE2
    }
    USE_SSSE3 {
        # SSSE3 implies SSE2:
        QMAKE_CFLAGS += -mssse3
        DEFINES += HAVE_SSE2 HAVE_SSSE3
    }

    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c \
        ../../$$ARCHDIR/lib/argon2-sse2.c \
        ../../$$ARCHDIR/lib/argon2-ssse3.c

    HEADERS += \
        ../../$$ARCHDIR/lib/argon2-sse2.h \
        ../../$$ARCHDIR/lib/argon2-ssse3.h
}
equals(ARCH, generic) {
    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
