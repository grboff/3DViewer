QT       += core gui widgets openglwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#LIBS += -lglu32
#LIBS += -lOpengl32 \
#        -lopengl32
LC_ALL=C qtcreator
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LC_ALL=C

SOURCES += \
    ../c_files/s21_view.c \
    main.cpp \
    mainwindow.cpp \
    myopenglwidget.cpp

HEADERS += \
    ../c_files/s21_view.h \
    mainwindow.h \
    myopenglwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../obj_files/cow.obj
