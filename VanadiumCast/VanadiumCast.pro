QT += core quick quickcontrols2 gui widgets multimedia network concurrent av avwidgets

CONFIG += qtquickcompiler

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

win32 {
    QMAKE_CXXFLAGS_RELEASE += /O2 /Oy
    CONFIG(release, debug|release) {
        message("release")
#        LIBS += -LE:/Dev/QtAV/build-release/lib_win_x86_64 -lQtAV1 -lQtAVWidgets1
#        LIBS += -LE:/Dev/Qt/5.15.2/msvc2019_64/lib -lQt5AV -lQt5AVWidgets
        LIBS += -L$$QT.core.libs -lQt5AV -lQt5AVWidgets
        LIBS += ../backend/release/backend.lib
        copyToDestDir($$OUT_PWD/../backend/release/backend.dll, $$OUT_PWD/release/)
    }
    CONFIG(debug, debug|release) {
        message("debug")
#        LIBS += -LE:/Dev/QtAV/build-debug/lib_win_x86_64 -lQtAVd1 -lQtAVWidgetsd1
        LIBS += -L$$QT.core.libs -lQt5AVd -lQt5AVWidgetsd
        LIBS += ../backend/debug/backend.lib
        copyToDestDir($$OUT_PWD/../backend/debug/backend.dll, $$OUT_PWD/debug/)
    }
}

unix {
    LIBS += ../backend/libbackend.so -lQtAV -lQtAVWidgets
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    copyToDestDir($$OUT_PWD/../backend/libbackend.so, $$OUT_PWD/)
    copyToDestDir($$OUT_PWD/../backend/libbackend.so.1, $$OUT_PWD/)
    copyToDestDir($$OUT_PWD/../backend/libbackend.so.1.0, $$OUT_PWD/)
    copyToDestDir($$OUT_PWD/../backend/libbackend.so.1.0.0, $$OUT_PWD/)
}

CONFIG += c++20

SOURCES += src/main.cpp

RESOURCES += res/resources.qrc

INCLUDEPATH += ../backend/src
