
# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = VanadiumCast

QT = core gui widgets quick quickcontrols2 av avwidgets network concurrent multimedia

HEADERS = \
   $$PWD/src/backend/API/API.h \
   $$PWD/src/backend/API/NetworkAPI.h \
   $$PWD/src/backend/API/SinkHandleWidget.h \
   $$PWD/src/backend/GUI/VideoGui.h \
   $$PWD/src/backend/GUI/VideoGuiLauncher.h \
   $$PWD/src/backend/GUI/WindowCloseEventFilter.h \
   $$PWD/src/backend/MediaProcessing/CachedLocalStream.h \
   $$PWD/src/backend/MediaProcessing/EncodingProfile.h \
   $$PWD/src/backend/MediaProcessing/FFMPEGTranscoder.h \
   $$PWD/src/backend/MediaProcessing/Input.h \
   $$PWD/src/backend/MediaProcessing/InputFile.h \
   $$PWD/src/backend/MediaProcessing/NetworkInput.h \
   $$PWD/src/backend/MediaProcessing/PlayerStateSlots.h \
   $$PWD/src/backend/MediaProcessing/VideoTranscoder.h \
   $$PWD/src/backend/Networking/CachedStream.h \
   $$PWD/src/backend/Networking/Device.h \
   $$PWD/src/backend/Networking/DeviceDirectory.h \
   $$PWD/src/backend/Networking/DeviceScan.h \
   $$PWD/src/backend/Networking/NetworkDevice.h \
   $$PWD/src/backend/Networking/NetworkDeviceDirectory.h \
   $$PWD/src/backend/Networking/NetworkDeviceScanner.h \
   $$PWD/src/backend/Networking/NetworkSinkHandler.h \
   $$PWD/src/backend/Networking/NetworkSinkTcpServer.h \
   $$PWD/src/backend/Networking/SinkHandler.h \
   $$PWD/src/backend/Networking/Streaming.h \
   $$PWD/src/backend/Networking/StreamThread.h \
   $$PWD/src/backend/Commands.h \
   $$PWD/src/backend/util.h \
#   $$PWD/test/MediaProcessing/CachedLocalStreamTest.h \
#   $$PWD/test/Networking/CachedStreamTest.h

SOURCES = \
   $$PWD/res/gui/fragments/PageDevices.qml \
   $$PWD/res/gui/fragments/PageMedia.qml \
   $$PWD/res/gui/fragments/PageStreaming.qml \
   $$PWD/res/gui/icons/pause.svg \
   $$PWD/res/gui/icons/play.svg \
   $$PWD/res/gui/model/DeviceListModel.qml \
   $$PWD/res/gui/main.qml \
   $$PWD/res/sound/incoming.wav \
   $$PWD/res/qml.qrc \
   $$PWD/res/qtquickcontrols2.conf \
   $$PWD/src/backend/API/NetworkAPI.cpp \
   $$PWD/src/backend/GUI/VideoGui.cpp \
   $$PWD/src/backend/GUI/VideoGui.ui \
   $$PWD/src/backend/GUI/VideoGuiLauncher.cpp \
   $$PWD/src/backend/GUI/WindowCloseEventFilter.cpp \
   $$PWD/src/backend/MediaProcessing/CachedLocalStream.cpp \
   $$PWD/src/backend/MediaProcessing/FFMPEGTranscoder.cpp \
   $$PWD/src/backend/MediaProcessing/InputFile.cpp \
   $$PWD/src/backend/MediaProcessing/NetworkInput.cpp \
   $$PWD/src/backend/MediaProcessing/VideoTranscoder.cpp \
   $$PWD/src/backend/Networking/CachedStream.cpp \
   $$PWD/src/backend/Networking/NetworkDevice.cpp \
   $$PWD/src/backend/Networking/NetworkDeviceDirectory.cpp \
   $$PWD/src/backend/Networking/NetworkDeviceScanner.cpp \
   $$PWD/src/backend/Networking/NetworkSinkHandler.cpp \
   $$PWD/src/backend/Networking/NetworkSinkTcpServer.cpp \
   $$PWD/src/backend/Networking/StreamThread.cpp \
#   $$PWD/src/main.cpp \
   $$PWD/src/backend/util.cpp \
#   $$PWD/test/MediaProcessing/CachedLocalStreamTest.cpp \
#   $$PWD/test/Networking/CachedStreamTest.cpp \
#   $$PWD/test/main.cpp

INCLUDEPATH = \
    $$PWD/src/backend \
    $$PWD/src/backend/API \
    $$PWD/src/backend/GUI \
    $$PWD/src/backend/MediaProcessing \
    $$PWD/src/backend/Networking \
#    $$PWD/test/MediaProcessing \
#    $$PWD/test/Networking

#DEFINES =

SUBDIRS += \
    QMake.old.pro


