TEMPLATE = lib

QT += core gui widgets network multimedia quick concurrent

# copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        unix: QMAKE_COPY_DIR += -P

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

unix: QMAKE_CXXFLAGS += -Werror=all -Werror=extra

unix: QMAKE_CXXFLAGS_RELEASE += -O4

win32: QMAKE_CXXFLAGS_RELEASE += /O2

win32 {
    QMAKE_CXXFLAGS_RELEASE += /O2 /Oy
    LIBS += -lOpenGL32
    CONFIG(release, debug|release) {
        message("release")
        message("$$QT.core.libs")
#        LIBS += -LE:/Dev/QtAV/build-release/lib_win_x86_64 -lQtAV1 -lQtAVWidgets1
        LIBS += -L$$QT.core.libs -lQt5AV -lQt5AVWidgets
        copyToDestDir($$OUT_PWD/release/backend.dll, $$OUT_PWD/../VanadiumCast/release/)
    }
    CONFIG(debug, debug|release) {
        message("debug")
        message("$$QT.core.libs")
#        LIBS += -LE:/Dev/QtAV/build-debug/lib_win_x86_64 -lQtAVd1 -lQtAVWidgetsd1
        LIBS += -L$$QT.core.libs -lQt5AVd -lQt5AVWidgetsd
        copyToDestDir($$OUT_PWD/debug/backend.dll, $$OUT_PWD/../VanadiumCast/debug/)
    }
}

unix {
    LIBS += -lQtAV -lQtAVWidgets
    QMAKE_LFLAGS_RPATH=
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    copyToDestDir($$OUT_PWD/libbackend.so, $$OUT_PWD/VanadiumCast/)
    copyToDestDir($$OUT_PWD/libbackend.so.1, $$OUT_PWD/VanadiumCast/)
    copyToDestDir($$OUT_PWD/libbackend.so.1.0, $$OUT_PWD/VanadiumCast/)
    copyToDestDir($$OUT_PWD/libbackend.so.1.0.0, $$OUT_PWD/VanadiumCast/)
}


CONFIG += c++20

SOURCES += src/util.cpp \
           src/API/NetworkAPI.cpp \
           src/GUI/VideoGuiLauncher.cpp \
           src/GUI/WindowCloseEventFilter.cpp \
           src/MediaProcessing/CachedLocalStream.cpp \
           src/MediaProcessing/InputFile.cpp \
           src/MediaProcessing/OGLUtil.cpp \
           src/MediaProcessing/VideoTranscoder.cpp \
           src/Networking/NetworkDevice.cpp \
           src/Networking/NetworkDeviceDirectory.cpp \
           src/Networking/NetworkDeviceScanner.cpp \
           src/Networking/NetworkSinkHandler.cpp \
           src/Networking/NetworkSinkTcpServer.cpp \
           src/Networking/StreamThread.cpp

HEADERS += src/util.h \
           src/Commands.h \
           src/API/NetworkAPI.h \
           src/GUI/VideoGuiLauncher.h \
           src/GUI/WindowCloseEventFilter.h \
           src/MediaProcessing/CachedLocalStream.h \
           src/MediaProcessing/InputFile.h \
           src/MediaProcessing/OGLUtil.h \
           src/MediaProcessing/VideoTranscoder.h \
           src/Networking/NetworkDevice.h \
           src/Networking/NetworkDeviceDirectory.h \
           src/Networking/NetworkDeviceScanner.h \
           src/Networking/NetworkSinkHandler.h \
           src/Networking/NetworkSinkTcpServer.h \
           src/Networking/StreamThread.h \
           src/Networking/SinkHandler.h \
           src/global.h
           src/Networking/DeviceScan.h
           src/Networking/DeviceDirectory.h
           src/Networking/Device.h
           src/MediaProcessing/Input.h

DEFINES += VCL_LIB

INCLUDEPATH += src/
