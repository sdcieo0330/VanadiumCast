cmake_minimum_required(VERSION 3.14)

project(VanadiumCast LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick avwidgets Widgets Network Multimedia Quick QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets avwidgets Network Multimedia Quick QuickControls2 REQUIRED)

if(MSVC)
    include_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\include")
    link_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\lib")
endif()

if(ANDROID)
    add_library(VanadiumCast SHARED
        main.cpp
        qml.qrc
        )
    add_library(VanadiumCastLib SHARED
        API/NetworkAPI.cpp
        MediaProcessing/Demuxer.cpp
        MediaProcessing/InputFile.cpp
        MediaProcessing/AudioDecoder.cpp
        MediaProcessing/AudioEncoder.cpp
        MediaProcessing/NetworkInput.cpp
        MediaProcessing/VideoDecoder.cpp
        MediaProcessing/VideoEncoder.cpp
        Networking/NetworkDevice.cpp
        Networking/NetworkStreamer.cpp
        Networking/NetworkSinkHandler.cpp
        Networking/NetworkSinkTcpServer.cpp
        Networking/NetworkDeviceScanner.cpp
        Networking/NetworkDeviceDirectory.cpp

        Commands.h
        API/API.h
        API/NetworkAPI.h
        API/SinkHandleWidget.h
        MediaProcessing/Input.h
        MediaProcessing/Codec.h
        MediaProcessing/Demux.h
        MediaProcessing/Demuxer.h
        MediaProcessing/Encoder.h
        MediaProcessing/Decoder.h
        MediaProcessing/InputFile.h
        MediaProcessing/AudioDecoder.h
        MediaProcessing/AudioEncoder.h
        MediaProcessing/NetworkInput.h
        MediaProcessing/VideoDecoder.h
        MediaProcessing/VideoEncoder.h
        Networking/Device.h
        Networking/Streaming.h
        Networking/DeviceScan.h
        Networking/SinkHandler.h
        Networking/NetworkDevice.h
        Networking/DeviceDirectory.h
        Networking/NetworkStreamer.h
        Networking/NetworkSinkHandler.h
        Networking/NetworkSinkTcpServer.h
        Networking/NetworkDeviceScanner.h
        Networking/NetworkDeviceDirectory.h
        )
else()
#    add_library(VanadiumCastLib SHARED
#        API/NetworkAPI.cpp
#        MediaProcessing/Demuxer.cpp
#        MediaProcessing/InputFile.cpp
#        MediaProcessing/AudioDecoder.cpp
#        MediaProcessing/AudioEncoder.cpp
#        MediaProcessing/NetworkInput.cpp
#        MediaProcessing/VideoDecoder.cpp
#        MediaProcessing/VideoEncoder.cpp
#        Networking/NetworkDevice.cpp
#        Networking/NetworkStreamer.cpp
#        Networking/NetworkSinkHandler.cpp
#        Networking/NetworkSinkTcpServer.cpp
#        Networking/NetworkDeviceScanner.cpp
#        Networking/NetworkDeviceDirectory.cpp

#        Commands.h
#        API/API.h
#        API/NetworkAPI.h
#        API/SinkHandleWidget.h
#        MediaProcessing/Input.h
#        MediaProcessing/Codec.h
#        MediaProcessing/Demux.h
#        MediaProcessing/Demuxer.h
#        MediaProcessing/Encoder.h
#        MediaProcessing/Decoder.h
#        MediaProcessing/InputFile.h
#        MediaProcessing/AudioDecoder.h
#        MediaProcessing/AudioEncoder.h
#        MediaProcessing/NetworkInput.h
#        MediaProcessing/VideoDecoder.h
#        MediaProcessing/VideoEncoder.h
#        Networking/Device.h
#        Networking/Streaming.h
#        Networking/DeviceScan.h
#        Networking/SinkHandler.h
#        Networking/NetworkDevice.h
#        Networking/DeviceDirectory.h
#        Networking/NetworkStreamer.h
#        Networking/NetworkSinkHandler.h
#        Networking/NetworkSinkTcpServer.h
#        Networking/NetworkDeviceScanner.h
#        Networking/NetworkDeviceDirectory.h
#        )
    add_executable(VanadiumCast
        API/NetworkAPI.cpp
        MediaProcessing/Demuxer.cpp
        MediaProcessing/InputFile.cpp
        MediaProcessing/AudioDecoder.cpp
        MediaProcessing/AudioEncoder.cpp
        MediaProcessing/NetworkInput.cpp
        MediaProcessing/VideoDecoder.cpp
        MediaProcessing/VideoEncoder.cpp
        Networking/NetworkDevice.cpp
        Networking/NetworkStreamer.cpp
        Networking/NetworkSinkHandler.cpp
        Networking/NetworkSinkTcpServer.cpp
        Networking/NetworkDeviceScanner.cpp
        Networking/NetworkDeviceDirectory.cpp

        Commands.h
        API/API.h
        API/NetworkAPI.h
        API/SinkHandleWidget.h
        MediaProcessing/Input.h
        MediaProcessing/Codec.h
        MediaProcessing/Demux.h
        MediaProcessing/Demuxer.h
        MediaProcessing/Encoder.h
        MediaProcessing/Decoder.h
        MediaProcessing/InputFile.h
        MediaProcessing/AudioDecoder.h
        MediaProcessing/AudioEncoder.h
        MediaProcessing/NetworkInput.h
        MediaProcessing/VideoDecoder.h
        MediaProcessing/VideoEncoder.h
        Networking/Device.h
        Networking/Streaming.h
        Networking/DeviceScan.h
        Networking/SinkHandler.h
        Networking/NetworkDevice.h
        Networking/DeviceDirectory.h
        Networking/NetworkStreamer.h
        Networking/NetworkSinkHandler.h
        Networking/NetworkSinkTcpServer.h
        Networking/NetworkDeviceScanner.h
        Networking/NetworkDeviceDirectory.h
        qml.qrc
        main.cpp
        )
endif()

#target_compile_definitions(VanadiumCastLib
#    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
#target_link_libraries(VanadiumCastLib
#    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(VanadiumCastLib PUBLIC avformat avcodec avdevice avfilter)

target_compile_definitions(VanadiumCast
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCast
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(VanadiumCast PRIVATE VanadiumCastLib)
target_link_libraries(VanadiumCast PUBLIC avformat avcodec avdevice avfilter)
