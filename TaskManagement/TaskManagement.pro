QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
RC_ICONS = TaskManagement_Logo.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addtasks.cpp \
    main.cpp \
    mainwindow.cpp \
    tache.cpp \
    tachecomposite.cpp \
    tacheterminale.cpp

HEADERS += \
    addtasks.h \
    mainwindow.h \
    tache.h \
    tachecomposite.h \
    tacheterminale.h

FORMS += \
    addtasks.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
