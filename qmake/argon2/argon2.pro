TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../src/run.c

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libargon2/release/ -largon2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libargon2/debug/ -largon2
else:unix: LIBS += -L$$OUT_PWD/../libargon2/ -largon2

INCLUDEPATH += $$PWD/../../include $$PWD/../../lib
DEPENDPATH  += $$PWD/../../include $$PWD/../../lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/release/ -largon2-arch
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libargon2-arch/debug/ -largon2-arch
else:unix: LIBS += -L$$OUT_PWD/../libargon2-arch/ -largon2-arch
