TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/bench2.c

HEADERS += \
    ../../src/timing.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libargon2/release/ -largon2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libargon2/debug/ -largon2
else:unix: LIBS += -L$$OUT_PWD/../libargon2/ -largon2

INCLUDEPATH += $$PWD/../../include
DEPENDPATH  += $$PWD/../../include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/release/ -largon2-arch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/debug/ -largon2-arch
else:unix: LIBS += -L$$OUT_PWD/../libargon2-arch/ -largon2-arch
