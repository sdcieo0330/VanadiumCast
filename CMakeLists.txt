cmake_minimum_required(VERSION 3.17)

project(VanadiumCast LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (!MSVC)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/cmake-3.18/Modules")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(test)
