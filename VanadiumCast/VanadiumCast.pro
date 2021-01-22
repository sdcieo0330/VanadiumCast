QT += core quick quickcontrols2 gui widgets multimedia network concurrent

CONFIG += qtquickcompiler

win32 {
    QMAKE_CXXFLAGS_RELEASE += /O2 /Oy
    CONFIG(release, debug|release) {
        message("release")
        LIBS += -LE:/Dev/QtAV/build-release/lib_win_x86_64 -lQtAV1 -lQtAVWidgets1
        LIBS += ../backend/release/backend.lib
    }
    CONFIG(debug, debug|release) {
        message("debug")
        LIBS += -LE:/Dev/QtAV/build-debug/lib_win_x86_64 -lQtAVd1 -lQtAVWidgetsd1
        LIBS += ../backend/debug/backend.lib
    }
}

unix {
    LIBS += ../backend/libbackend.so -lQtAV -lQtAVWidgets
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

CONFIG += c++20

SOURCES += src/main.cpp

RESOURCES += res/resources.qrc

INCLUDEPATH += ../backend/src
