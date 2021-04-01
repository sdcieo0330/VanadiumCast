set(PROJECT_NAME backend)

################################################################################
# Source groups
################################################################################
set(Generated_Files
#    "debug/moc_CachedLocalStream.cpp"
#    "debug/moc_NetworkAPI.cpp"
#    "debug/moc_NetworkDevice.cpp"
#    "debug/moc_NetworkDeviceDirectory.cpp"
#    "debug/moc_NetworkSinkHandler.cpp"
#    "debug/moc_NetworkSinkTcpServer.cpp"
#    "debug/moc_OGLUtil.cpp"
#    "debug/moc_predefs.h.cbt"
#    "debug/moc_ScreenRecorder.cpp"
#    "debug/moc_StreamThread.cpp"
#    "debug/moc_VideoGuiLauncher.cpp"
#    "debug/moc_VideoTranscoder.cpp"
#    "debug/moc_WindowCloseEventFilter.cpp"
#    "release/moc_CachedLocalStream.cpp"
#    "release/moc_NetworkAPI.cpp"
#    "release/moc_NetworkDevice.cpp"
#    "release/moc_NetworkDeviceDirectory.cpp"
#    "release/moc_NetworkSinkHandler.cpp"
#    "release/moc_NetworkSinkTcpServer.cpp"
#    "release/moc_OGLUtil.cpp"
#    "release/moc_predefs.h.cbt"
#    "release/moc_ScreenRecorder.cpp"
#    "release/moc_StreamThread.cpp"
#    "release/moc_VideoGuiLauncher.cpp"
#    "release/moc_VideoTranscoder.cpp"
#    "release/moc_WindowCloseEventFilter.cpp"
)
source_group("Generated Files" FILES ${Generated_Files})

set(Header_Files
    "src/API/NetworkAPI.h"
    "src/Commands.h"
    "src/global.h"
    "src/GUI/VideoGuiLauncher.h"
    "src/GUI/WindowCloseEventFilter.h"
    "src/MediaProcessing/CachedLocalStream.h"
    "src/MediaProcessing/InputFile.h"
    "src/MediaProcessing/oglutil.h"
    "src/MediaProcessing/ScreenRecorder.h"
    "src/MediaProcessing/VideoTranscoder.h"
    "src/Networking/NetworkDevice.h"
    "src/Networking/NetworkDeviceDirectory.h"
    "src/Networking/NetworkDeviceScanner.h"
    "src/Networking/NetworkSinkHandler.h"
    "src/Networking/NetworkSinkTcpServer.h"
    "src/Networking/SinkHandler.h"
    "src/Networking/StreamThread.h"
    "src/util.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/API/NetworkAPI.cpp"
    "src/GUI/VideoGuiLauncher.cpp"
    "src/GUI/WindowCloseEventFilter.cpp"
    "src/MediaProcessing/CachedLocalStream.cpp"
    "src/MediaProcessing/InputFile.cpp"
    "src/MediaProcessing/oglutil.cpp"
    "src/MediaProcessing/ScreenRecorder.cpp"
    "src/MediaProcessing/VideoTranscoder.cpp"
    "src/Networking/NetworkDevice.cpp"
    "src/Networking/NetworkDeviceDirectory.cpp"
    "src/Networking/NetworkDeviceScanner.cpp"
    "src/Networking/NetworkSinkHandler.cpp"
    "src/Networking/NetworkSinkTcpServer.cpp"
    "src/Networking/StreamThread.cpp"
    "src/util.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Generated_Files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE backend)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Qt4VSv1.0"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "backend"
    TARGET_NAME_RELEASE "backend"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/debug"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/release"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/debug"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/release"
    ">"
    "${CMAKE_CURRENT_SOURCE_DIR}/.;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtWidgets;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtMultimedia;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtQuick;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtGui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtANGLE;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtQmlModels;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtQml;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtNetwork;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtConcurrent;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtCore;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanSDK/1.2.162.1/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/mkspecs/win32-msvc"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "QT_NO_DEBUG"
    ">"
    "_WINDOWS;"
    "UNICODE;"
    "_UNICODE;"
    "WIN32;"
    "_ENABLE_EXTENDED_ALIGNED_STORAGE;"
    "WIN64;"
    "VCL_LIB;"
    "QT_WIDGETS_LIB;"
    "QT_MULTIMEDIA_LIB;"
    "QT_QUICK_LIB;"
    "QT_GUI_LIB;"
    "QT_QMLMODELS_LIB;"
    "QT_QML_LIB;"
    "QT_NETWORK_LIB;"
    "QT_CONCURRENT_LIB;"
    "QT_CORE_LIB"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /W3;
        /wd4577;
        /wd4467;
        -Zc:rvalueCast;
        -Zc:inline;
        -Zc:strictStrings;
        -Zc:throwingNew;
        -Zc:referenceBinding;
        -Zc:__cplusplus;
        -w34100;
        -w34189;
        -w44996;
        -w44456;
        -w44457;
        -w44458;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Zc:wchar_t
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /INCREMENTAL:NO
        >
        /NOLOGO;
        /SUBSYSTEM:WINDOWS;
        /NXCOMPAT;
        /DYNAMICBASE
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:Debug> xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\debug\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\..\\VanadiumCast\\debug\\
    COMMAND   $<CONFIG:Debug> if errorlevel 1 goto VCEnd
    COMMAND $<CONFIG:Release> xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\release\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\..\\VanadiumCast\\release\\
    COMMAND $<CONFIG:Release> if errorlevel 1 goto VCEnd
    COMMENT "xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\debug\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\backend\\..\\VanadiumCast\\debug\\ 
"
)

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVWidgetsd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Widgetsd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Multimediad;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Quickd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Guid;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QmlModelsd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Qmld;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Networkd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Concurrentd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Cored"
    ">"
    "$<$<CONFIG:Release>:"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AV;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVWidgets;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Widgets;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Multimedia;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Quick;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Gui;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QmlModels;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Qml;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Network;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Concurrent;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Core"
    ">"
    "OpenGL32;"
    "d3d9"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "F:/Dev/Qt/5.15.2/msvc2019_64/lib"
)

