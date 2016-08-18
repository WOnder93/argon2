#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T12:02:48
#
#-------------------------------------------------

QT       -= core gui

TARGET = argon2-arch
TEMPLATE = lib

isEmpty(ARCH) {
    warning("ARCH not set, using 'generic'!")
    ARCH = generic
}

ARCHDIR = arch/$$ARCH

INCLUDEPATH += ../../include ../../lib
INCLUDEPATH +=  ../../$$ARCHDIR/lib

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
    USE_XOP {
        # XOP implies SSSE3 and SSE2:
        QMAKE_CFLAGS += -mxop
        DEFINES += HAVE_SSE2 HAVE_SSSE3 HAVE_XOP
    }

    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c \
        ../../$$ARCHDIR/lib/argon2-sse2.c \
        ../../$$ARCHDIR/lib/argon2-ssse3.c \
        ../../$$ARCHDIR/lib/argon2-xop.c

    HEADERS += \
        ../../$$ARCHDIR/lib/argon2-sse2.h \
        ../../$$ARCHDIR/lib/argon2-ssse3.h \
        ../../$$ARCHDIR/lib/argon2-xop.h
}
equals(ARCH, generic) {
    SOURCES += \
        ../../$$ARCHDIR/lib/argon2-arch.c
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
