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

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick QuickControls2 Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 Widgets LinguistTools REQUIRED)

set(TS_FILES VanadiumCast_en_DE.ts)

include_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\include")
link_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\lib")

if(ANDROID)
    add_library(VanadiumCast SHARED
      main.cpp
      API/API.h
      API/NetworkAPI.cpp
      API/NetworkAPI.h
      API/SinkHandleWidget.h
      MediaProcessing/Codec.h
      MediaProcessing/Encoder.h
      MediaProcessing/Decoder.h
      MediaProcessing/AudioDecoder.h
      MediaProcessing/AudioEncoder.h
      MediaProcessing/AudioDecode.h
      MediaProcessing/AudioEncode.h
      MediaProcessing/Demuxer.h
      MediaProcessing/Demux.h
      MediaProcessing/Input.h
      MediaProcessing/InputFile.h
      MediaProcessing/NetworkInput.h
      MediaProcessing/VideoDecode.h
      MediaProcessing/VideoEncode.h
      MediaProcessing/VideoDecoder.h
      MediaProcessing/VideoEncoder.h
      MediaProcessing/AudioDecoder.cpp
      MediaProcessing/AudioEncoder.cpp
      MediaProcessing/Demuxer.cpp
      MediaProcessing/InputFile.cpp
      MediaProcessing/VideoDecoder.cpp
      MediaProcessing/VideoEncoder.cpp
      Networking/NetworkDeviceDirectory.h
      Networking/NetworkDeviceDirectory.h
      Networking/NetworkDeviceScanner.h
      Networking/NetworkSinkHandler.h
      Networking/NetworkStreamer.h
      Networking/DeviceDirectory.h
      Networking/NetworkDevice.h
      Networking/SinkHandler.h
      Networking/DeviceScan.h
      Networking/Streaming.h
      Networking/Device.h
      Networking/NetworkDeviceDirectory.cpp
      Networking/NetworkDeviceScanner.cpp
      Networking/NetworkSinkHandler.cpp
      Networking/NetworkStreamer.cpp
      Networking/NetworkDevice.cpp
      qml.qrc
      ${TS_FILES}
    )
else()
    add_executable(VanadiumCast
      main.cpp
      API/API.h
      API/NetworkAPI.cpp
      API/NetworkAPI.h
      API/SinkHandleWidget.h
      MediaProcessing/Codec.h
      MediaProcessing/Encoder.h
      MediaProcessing/Decoder.h
      MediaProcessing/AudioDecoder.h
      MediaProcessing/AudioEncoder.h
      MediaProcessing/AudioDecode.h
      MediaProcessing/AudioEncode.h
      MediaProcessing/Demuxer.h
      MediaProcessing/Demux.h
      MediaProcessing/Input.h
      MediaProcessing/InputFile.h
      MediaProcessing/NetworkInput.h
      MediaProcessing/VideoDecode.h
      MediaProcessing/VideoEncode.h
      MediaProcessing/VideoDecoder.h
      MediaProcessing/VideoEncoder.h
      MediaProcessing/AudioDecoder.cpp
      MediaProcessing/AudioEncoder.cpp
      MediaProcessing/Demuxer.cpp
      MediaProcessing/InputFile.cpp
      MediaProcessing/VideoDecoder.cpp
      MediaProcessing/VideoEncoder.cpp
      Networking/NetworkDeviceDirectory.h
      Networking/NetworkDeviceDirectory.h
      Networking/NetworkDeviceScanner.h
      Networking/NetworkSinkHandler.h
      Networking/NetworkStreamer.h
      Networking/DeviceDirectory.h
      Networking/NetworkDevice.h
      Networking/SinkHandler.h
      Networking/DeviceScan.h
      Networking/Streaming.h
      Networking/Device.h
      Networking/NetworkDeviceDirectory.cpp
      Networking/NetworkDeviceScanner.cpp
      Networking/NetworkSinkHandler.cpp
      Networking/NetworkStreamer.cpp
      Networking/NetworkDevice.cpp
      qml.qrc
      ${TS_FILES}
    )
endif()

target_compile_definitions(VanadiumCast
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCast
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2 Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
