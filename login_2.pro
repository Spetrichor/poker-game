QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Card.cpp \
    Player.cpp \
    dialog2.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    play.cpp \
    rule.cpp

HEADERS += \
    Array.h \
    Card.h \
    Player.h \
    dialog2.h \
    game.h \
    mainwindow.h \
    play.h \
    rule.h

FORMS += \
    dialog2.ui \
    game.ui \
    mainwindow.ui \
    play.ui \
    rule.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
RC_FILE = puk.rc
