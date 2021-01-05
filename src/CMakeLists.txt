cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    include_directories(E:\\QtAV\\src\\QtAV E:\\QtAV\\widgets\\QtAVWidgets E:\\ffmpeg-dev\\include)
    link_directories(E:\\QtAV\\x64\\Debug E:\\ffmpeg-dev\\lib)
else ()
    set(CMAKE_MODULE_PATH /usr/share/cmake-3.18/Modules /usr/share/cmake-3.19/Modules ${CMAKE_MODULE_PATH})
    find_package(QtAV REQUIRED)
endif ()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Widgets Network Multimedia QuickControls2 Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Widgets Network Multimedia QuickControls2 Concurrent REQUIRED)

#include_directories(${QTAV_INCLUDE_DIRS} ${QTAVWIDGETS_INCLUDE_DIRS})
add_library(VanadiumCastLib STATIC
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
        Networking/CachedStream.cpp Networking/CachedStream.h MediaProcessing/CachedLocalStream.cpp MediaProcessing/CachedLocalStream.h util.cpp util.h MediaProcessing/FFMPEGTranscoder.cpp MediaProcessing/FFMPEGTranscoder.h GUI/WindowCloseEventFilter.cpp GUI/WindowCloseEventFilter.h)
add_executable(VanadiumCast
        ../res/qml.qrc
        main.cpp)

target_compile_definitions(VanadiumCastLib
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCastLib
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

target_compile_definitions(VanadiumCast
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(VanadiumCast
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)

if (MSVC)
    target_link_libraries(VanadiumCastLib PRIVATE QtAV QtAVWidgets)
    target_link_libraries(VanadiumCast PRIVATE QtAV QtAVWidgets QmlAV)
else ()
    target_link_libraries(VanadiumCastLib PRIVATE QtAV QtAVWidgets)
    target_link_libraries(VanadiumCast PRIVATE QtAV QtAVWidgets)
endif ()

target_link_libraries(VanadiumCast PRIVATE VanadiumCastLib)

#target_link_libraries(VanadiumCast PRIVATE VanadiumCastLib)
target_link_libraries(VanadiumCastLib PRIVATE avformat avcodec avdevice avfilter)
