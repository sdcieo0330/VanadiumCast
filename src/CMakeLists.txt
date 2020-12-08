cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_link_options(-static-libasan)

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

find_package(QtAV REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets Network Multimedia QuickControls2 Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets Network Multimedia QuickControls2 Concurrent REQUIRED)

if(MSVC)
    include_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\include")
    link_directories("C:\\Users\\Silas\\Documents\\ffmpeg-dev\\lib")
endif()

include_directories(${QTAV_INCLUDE_DIRS} ${QTAVWIDGETS_INCLUDE_DIRS})
    add_library(VanadiumCastLib SHARED
            API/NetworkAPI.cpp
            GUI/VideoGui.cpp
            GUI/VideoGuiLauncher.cpp
            MediaProcessing/InputFile.cpp
            MediaProcessing/NetworkInput.cpp
            MediaProcessing/VideoTranscoder.cpp
            Networking/NetworkDevice.cpp
            Networking/StreamThread.cpp
            Networking/NetworkSinkHandler.cpp
            Networking/NetworkSinkTcpServer.cpp
            Networking/NetworkDeviceScanner.cpp
            Networking/NetworkDeviceDirectory.cpp

            Commands.h
            API/API.h
            API/NetworkAPI.h
            API/SinkHandleWidget.h
            GUI/VideoGui.h
            GUI/VideoGuiLauncher.h
            MediaProcessing/Input.h
            MediaProcessing/InputFile.h
            MediaProcessing/NetworkInput.h
            MediaProcessing/EncodingProfile.h
            MediaProcessing/VideoTranscoder.h
            MediaProcessing/PlayerStateSlots.h
            Networking/Device.h
            Networking/Streaming.h
            Networking/DeviceScan.h
            Networking/SinkHandler.h
            Networking/NetworkDevice.h
            Networking/DeviceDirectory.h
            Networking/StreamThread.h
            Networking/NetworkSinkHandler.h
            Networking/NetworkSinkTcpServer.h
            Networking/NetworkDeviceScanner.h
            Networking/NetworkDeviceDirectory.h
            Networking/CachedStream.cpp Networking/CachedStream.h MediaProcessing/CachedLocalStream.cpp MediaProcessing/CachedLocalStream.h util.cpp util.h MediaProcessing/FFMPEGTranscoder.cpp MediaProcessing/FFMPEGTranscoder.h)
add_executable(VanadiumCast
        ../res/qml.qrc
        main.cpp)

target_compile_definitions(VanadiumCastLib
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCastLib
    PRIVATE QtAV QtAVWidgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

target_compile_definitions(VanadiumCast
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCast
        PRIVATE QtAV QtAVWidgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(VanadiumCast PRIVATE VanadiumCastLib)

#target_link_libraries(VanadiumCast PRIVATE VanadiumCastLib)
target_link_libraries(VanadiumCastLib PUBLIC avformat avcodec avdevice avfilter)
