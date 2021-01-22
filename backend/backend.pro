TEMPLATE = lib

QT += core gui widgets network multimedia quick concurrent

win32 {
    QMAKE_CXXFLAGS_RELEASE += /O2 /Oy
    CONFIG(release, debug|release) {
        message("release")
        LIBS += -LE:/Dev/QtAV/build-release/lib_win_x86_64 -lQtAV1 -lQtAVWidgets1
    }
    CONFIG(debug, debug|release) {
        message("debug")
        LIBS += -LE:/Dev/QtAV/build-debug/lib_win_x86_64 -lQtAVd1 -lQtAVWidgetsd1
    }
}

unix {
    QT += av avwidgets
}


CONFIG += c++20

SOURCES += src/util.cpp \
           src/API/NetworkAPI.cpp \
           src/GUI/VideoGuiLauncher.cpp \
           src/GUI/WindowCloseEventFilter.cpp \
           src/MediaProcessing/CachedLocalStream.cpp \
           src/MediaProcessing/InputFile.cpp \
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
