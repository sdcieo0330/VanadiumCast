QT += quick core gui widgets network avwidgets

windows: INCLUDEPATH += C:\\Users\\Silas\\Documents\\ffmpeg-dev\\include

windows: QMAKE_LIBDIR += C:\\Users\\Silas\\Documents\\ffmpeg-dev\\lib

LIBS += -lavformat -lavcodec -lavdevice -lavfilter -lQtAVWidgets -lQtAV

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    API/NetworkAPI.cpp \
    MediaProcessing/Demuxer.cpp \
    MediaProcessing/InputFile.cpp \
    MediaProcessing/AudioDecoder.cpp \
    MediaProcessing/AudioEncoder.cpp \
    MediaProcessing/NetworkInput.cpp \
    MediaProcessing/VideoDecoder.cpp \
    MediaProcessing/VideoEncoder.cpp \
    MediaProcessing/VideoTranscoder.cpp \
    Networking/NetworkDevice.cpp \
    Networking/NetworkStreamer.cpp \
    Networking/NetworkSinkHandler.cpp \
    Networking/NetworkSinkTcpServer.cpp \
    Networking/NetworkDeviceScanner.cpp \
    Networking/NetworkDeviceDirectory.cpp \
    GUI/VideoGui.cpp

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
    Commands.h \
    API/API.h \
    API/NetworkAPI.h \
    API/SinkHandleWidget.h \
    MediaProcessing/EncodingProfile.h \
    MediaProcessing/Input.h \
    MediaProcessing/Codec.h \
    MediaProcessing/Demux.h \
    MediaProcessing/Demuxer.h \
    MediaProcessing/Encoder.h \
    MediaProcessing/Decoder.h \
    MediaProcessing/InputFile.h \
    MediaProcessing/AudioDecoder.h \
    MediaProcessing/AudioEncoder.h \
    MediaProcessing/NetworkInput.h \
    MediaProcessing/PlayerStateSlots.h \
    MediaProcessing/VideoDecoder.h \
    MediaProcessing/VideoEncoder.h \
    MediaProcessing/VideoTranscoder.h \
    Networking/Device.h \
    Networking/Streaming.h \
    Networking/DeviceScan.h \
    Networking/SinkHandler.h \
    Networking/NetworkDevice.h \
    Networking/DeviceDirectory.h \
    Networking/NetworkStreamer.h \
    Networking/NetworkSinkHandler.h \
    Networking/NetworkSinkTcpServer.h \
    Networking/NetworkDeviceScanner.h \
    Networking/NetworkDeviceDirectory.h \
    GUI/VideoGui.h

FORMS += \
    GUI/VideoGui.ui
