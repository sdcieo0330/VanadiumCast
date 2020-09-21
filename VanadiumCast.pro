QT += quick core gui widgets network

windows: INCLUDEPATH += C:\\Users\\Silas\\Documents\\ffmpeg-dev\\include

windows: QMAKE_LIBDIR += C:\\Users\\Silas\\Documents\\ffmpeg-dev\\lib

LIBS += -lavformat -lavcodec -lavdevice -lavfilter

CONFIG += c++20
CONFIG += qmltypes
QML_IMPORT_NAME = silas.vanadiumcast
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        API/NetworkAPI.cpp \
        MediaProcessing/AudioDecoder.cpp \
        MediaProcessing/AudioEncoder.cpp \
        MediaProcessing/Demuxer.cpp \
        MediaProcessing/InputFile.cpp \
        MediaProcessing/NetworkInput.cpp \
        MediaProcessing/VideoDecoder.cpp \
        MediaProcessing/VideoEncoder.cpp \
        Networking/NetworkDevice.cpp \
        Networking/NetworkDeviceDirectory.cpp \
        Networking/NetworkDeviceScanner.cpp \
        Networking/NetworkSinkHandler.cpp \
        Networking/NetworkSinkTcpServer.cpp \
        Networking/NetworkStreamer.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    API/API.h \
    API/NetworkAPI.h \
    API/SinkHandleWidget.h \
    Commands.h \
    MediaProcessing/AudioDecode.h \
    MediaProcessing/AudioDecoder.h \
    MediaProcessing/AudioEncode.h \
    MediaProcessing/AudioEncoder.h \
    MediaProcessing/Codec.h \
    MediaProcessing/Decoder.h \
    MediaProcessing/Demux.h \
    MediaProcessing/Demuxer.h \
    MediaProcessing/Encoder.h \
    MediaProcessing/Input.h \
    MediaProcessing/InputFile.h \
    MediaProcessing/NetworkInput.h \
    MediaProcessing/VideoDecode.h \
    MediaProcessing/VideoDecoder.h \
    MediaProcessing/VideoEncode.h \
    MediaProcessing/VideoEncoder.h \
    Networking/Device.h \
    Networking/DeviceDirectory.h \
    Networking/DeviceScan.h \
    Networking/NetworkDevice.h \
    Networking/NetworkDeviceDirectory.h \
    Networking/NetworkDeviceScanner.h \
    Networking/NetworkSinkHandler.h \
    Networking/NetworkSinkTcpServer.h \
    Networking/NetworkStreamer.h \
    Networking/SinkHandler.h \
    Networking/Streaming.h
