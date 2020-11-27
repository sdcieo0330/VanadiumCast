cmake_minimum_required(VERSION 3.5)

project(VanadiumCastTest LANGUAGES CXX)


include_directories(../src)

find_package(Qt5 COMPONENTS Core Test Gui Widgets Concurrent Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(VanadiumCastTest Networking/CachedStreamTest.cpp)
add_test(NAME VanadiumCastTest COMMAND VanadiumCastTest)

target_link_libraries(VanadiumCastTest PRIVATE VanadiumCastLib Qt5::Gui Qt5::Test Qt5::Core Qt5::Widgets Qt5::Concurrent Qt5::Network)
