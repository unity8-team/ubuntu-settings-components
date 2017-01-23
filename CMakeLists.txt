project(settings-components)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Install paths
include(GNUInstallDirs)

include(FindPkgConfig)

include(FindCups)

find_package(Qt5Quick REQUIRED)

set(QT_IMPORTS_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/qml)

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)

if(NOT CUPS_FOUND)
message(FATAL_ERROR "Could not find cups.")
endif()

add_definitions(-DQT_NO_KEYWORDS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# enable QML debugging
string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_lower) # Build types should always be lowercase but sometimes they are not.
if(cmake_build_type_lower MATCHES "debug")
    add_definitions(-DQT_QML_DEBUG)
endif()

# Quiet logger
set(QUIET_LOGGER OFF CACHE BOOL "Disable test output on stdout.")
if(NOT QUIET_LOGGER)
    set(STDOUT_LOGGER "-o" "-,txt")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "relwithdebinfo")
    option(Werror "Treat warnings as errors" ON)
else()
    option(Werror "Treat warnings as errors" OFF)
endif()

if (Werror)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

file(GLOB_RECURSE QML_EXTRA_FILES
    ${CMAKE_SOURCE_DIR}/tests/*.qml
    ${CMAKE_SOURCE_DIR}/tests/*.js
    ${CMAKE_SOURCE_DIR}/tests/*.jpg
    ${CMAKE_SOURCE_DIR}/tests/*.png
    ${CMAKE_SOURCE_DIR}/tests/*.sci
    ${CMAKE_SOURCE_DIR}/tests/*.svg
)
add_custom_target(qml_fake_target SOURCES ${QML_EXTRA_FILES})

# Tests
enable_testing()

add_subdirectory(plugins)
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(po)
