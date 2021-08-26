QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    editor.cpp \
    main.cpp \
    mainwindow.cpp \
    menuentry.cpp \
    settings.cpp

HEADERS += \
    app_info.h \
    editor.h \
    mainwindow.h \
    menuentry.h \
    settings.h

FORMS += \
    editor.ui \
    mainwindow.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
