set(PROJECT_NAME VanadiumCast)

################################################################################
# Source groups
################################################################################
set(Generated_Files
#    "debug/moc_predefs.h.cbt"
#    "debug/qmlcache_loader.cpp"
#    "debug/qrc_res_resources_qmlcache.cpp"
#    "debug/res_gui_fragments_PageDevices_qml.cpp"
#    "debug/res_gui_fragments_PageMedia_qml.cpp"
#    "debug/res_gui_fragments_PageStreaming_qml.cpp"
#    "debug/res_gui_main_qml.cpp"
#    "debug/res_gui_model_DeviceListModel_qml.cpp"
#    "release/moc_predefs.h.cbt"
#    "release/qmlcache_loader.cpp"
#    "release/qrc_res_resources_qmlcache.cpp"
#    "release/res_gui_fragments_PageDevices_qml.cpp"
#    "release/res_gui_fragments_PageMedia_qml.cpp"
#    "release/res_gui_fragments_PageStreaming_qml.cpp"
#    "release/res_gui_main_qml.cpp"
#    "release/res_gui_model_DeviceListModel_qml.cpp"
)
source_group("Generated Files" FILES ${Generated_Files})

set(Resource_Files
    "res/gui/fragments/PageDevices.qml"
    "res/gui/fragments/PageMedia.qml"
    "res/gui/fragments/PageStreaming.qml"
    "res/gui/icons/pause.svg"
    "res/gui/icons/play.svg"
    "res/gui/main.qml"
    "res/gui/model/DeviceListModel.qml"
    "res/qtquickcontrols2.conf"
    "res/sound/incoming.wav"
    "res_resources_qmlcache.qrc"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
    "src/main.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(qmlcachengen_loader_res/resources.qrc
    "res/resources.qrc"
)
source_group("qmlcachengen_loader res/resources.qrc" FILES ${qmlcachengen_loader_res/resources.qrc})

set(ALL_FILES
    ${Generated_Files}
    ${Resource_Files}
    ${Source_Files}
    ${qmlcachengen_loader_res/resources.qrc}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE VanadiumCast)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Qt4VSv1.0"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "VanadiumCast"
    TARGET_NAME_RELEASE "VanadiumCast"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../backend/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtQuickControls2;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtQuick;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtOpenGL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtWidgets;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Qt/5.15.2/msvc2019_64/include/QtMultimedia;"
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
    "QT_QUICKCONTROLS2_LIB;"
    "QT_QUICK_LIB;"
    "QT_OPENGL_LIB;"
    "QT_WIDGETS_LIB;"
    "QT_MULTIMEDIA_LIB;"
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
        /DYNAMICBASE;
        "/MANIFESTDEPENDENCY:type='win32';
        name='Microsoft.Windows.Common-Controls';
        version='6.0.0.0';
        publicKeyToken='6595b64144ccf1df';
        language='*';
        processorArchitecture='*'"
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:Debug> xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\..\\backend\\debug\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\debug\\
    COMMAND   $<CONFIG:Debug> if errorlevel 1 goto VCEnd
    COMMAND $<CONFIG:Release> xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\..\\backend\\release\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\release\\
    COMMAND $<CONFIG:Release> if errorlevel 1 goto VCEnd
    COMMENT "xcopy /s /q /y /i F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\..\\backend\\debug\\backend.dll F:\\Dev\\CLionProjects\\VanadiumCast\\VanadiumCast\\debug\\ 
"
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    backend
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVWidgetsd;"
        "..\\backend\\debug\\backend;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QuickControls2d;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Quickd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5OpenGLd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Widgetsd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Multimediad;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Guid;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QmlModelsd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Qmld;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Networkd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Concurrentd;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Cored;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\qtmaind"
    ">"
    "$<$<CONFIG:Release>:"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AV;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5AVWidgets;"
        "..\\backend\\release\\backend;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QuickControls2;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Quick;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5OpenGL;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Widgets;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Multimedia;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Gui;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5QmlModels;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Qml;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Network;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Concurrent;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\Qt5Core;"
        "F:\\Dev\\Qt\\5.15.2\\msvc2019_64\\lib\\qtmain"
    ">"
    "OpenGL32;"
    "shell32"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "F:/Dev/Qt/5.15.2/msvc2019_64/lib;"
    "C:/openssl/lib;"
    "C:/Utils/my_sql/mysql-5.7.25-winx64/lib;"
    "C:/Utils/postgresql/pgsql/lib"
)

