project(UbuntuSettingsComponentsQml)

find_package(Qt5Core REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GLIB_INCLUDE_DIRS}
)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

add_library(UbuntuSettingsComponentsQml MODULE
    plugin.cpp
    filepickerhelper.cpp
    serverpropertysynchroniser.cpp
)
target_link_libraries(UbuntuSettingsComponentsQml ${GLIB_LDFLAGS})
qt5_use_modules(UbuntuSettingsComponentsQml Core Qml Quick)

add_usc_plugin(Ubuntu.Settings.Components 0.1 Ubuntu/Settings/Components TARGETS UbuntuSettingsComponentsQml)
